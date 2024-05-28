#include "sky_physics.h"

#include <chrono>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string.h>

#include "cmath"
#include "glm/vec3.hpp"
#include "glm/detail/_noise.hpp"
#include "glm/detail/_noise.hpp"
#include "glm/detail/_noise.hpp"
#include "glm/detail/_noise.hpp"
#include "glm/detail/_noise.hpp"
#include "glm/detail/_noise.hpp"
#include "glm/ext/quaternion_exponential.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtx/intersect.hpp"
#include "glm/gtx/vector_angle.inl"

// double atmosphere::molecular_density(const double height) const {
//     return sea_level_molecular_density * exp(-height / atmospheric_thickness);
// }

glm::vec3 atmosphere::beta_rayleigh_computed(const float height) const {
    return
    8 * powf(M_PI, 3) * powf(powf(air_refraction_index, 2) - 1, 2) /
    (3 * molecular_density * pow(wavelength_peak, glm::vec3(4))) *
    expf(-height / rayleigh_scale_height);
}

// double atmosphere::rayleigh_extinction_coefficient(const double height, const double wavelength) const {
//     return beta_rayleigh_computed(height, wavelength);
// }

float atmosphere::rayleigh_phase(const glm::vec3 view_direction) const {
    return 3.0f / (16.0f * M_PI) * (1 + powf(dot(view_direction, sun_direction), 2));
}

// float atmosphere::mie_phase_function(const glm::vec3 light_direction, const glm::vec3 view_direction) const {
//     const double my = cos_sim(light_direction, view_direction);
//     return
//     3 / (8 * M_PI) *
//     ((1-pow(medium_anisotropy, 2)) * (1 + pow(my, 2))) /
//     ((2+pow(medium_anisotropy, 2))*pow(1 + pow(medium_anisotropy, 2) - 2 * medium_anisotropy * my, 3.0/2.0)) ;
// }

float atmosphere::cos_sim(const glm::vec3 a, const glm::vec3 b) {
    return cosf(angle(a, b));
}

//TODO Optical Depth

void renderSkydome(const glm::vec3 sunDir, const char *filename) {
    atmosphere atmosphere(sunDir);

    auto t0 = std::chrono::high_resolution_clock::now();
    const unsigned width = 512, height = 512;
    glm::vec3 *image = new glm::vec3[width * height], *p = image;
    memset(image, 0x0, sizeof(glm::vec3) * width * height);

    for (unsigned j = 0; j < height; ++j) {
        float y = 2.f * (j + 0.5f) / float(height - 1) - 1.f;
        for (unsigned i = 0; i < width; ++i, ++p) {
            float x = 2.f * (i + 0.5f) / float(width - 1) - 1.f;
            float z2 = x * x + y * y;
            if (z2 <= 1) {
                float phi = std::atan2(y, x);
                float theta = std::acos(1 - z2);
                glm::vec3 orig(0, atmosphere.atmosphere_radius + 1, 0);
                glm::vec3 dir(std::sin(theta) * std::cos(phi), std::cos(theta), std::sin(theta) * std::sin(phi));
                // 1 meter above sea level
                *p = atmosphere.incident_light(orig, dir);
            }
        }
        fprintf(stderr, "\b\b\b\b\%3d%c", (int)(100 * j / (width - 1)), '%');
    }

    std::cout << "\b\b\b\b" << ((std::chrono::duration<float>)(std::chrono::high_resolution_clock::now() - t0)).count() << " seconds" << std::endl;
    // Save result to a PPM image (keep these flags if you compile under Windows)
    std::ofstream ofs(filename, std::ios::out | std::ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    p = image;
    for (unsigned j = 0; j < height; ++j) {
        for (unsigned i = 0; i < width; ++i, ++p) {
#if 1
            // Apply tone mapping function
            (*p)[0] = (*p)[0] < 1.413f ? pow((*p)[0] * 0.38317f, 1.0f / 2.2f) : 1.0f - exp(-(*p)[0]);
            (*p)[1] = (*p)[1] < 1.413f ? pow((*p)[1] * 0.38317f, 1.0f / 2.2f) : 1.0f - exp(-(*p)[1]);
            (*p)[2] = (*p)[2] < 1.413f ? pow((*p)[2] * 0.38317f, 1.0f / 2.2f) : 1.0f - exp(-(*p)[2]);
#endif
            ofs << (unsigned char)(std::min(1.f, (*p)[0]) * 255)
                << (unsigned char)(std::min(1.f, (*p)[1]) * 255)
                << (unsigned char)(std::min(1.f, (*p)[2]) * 255);
        }
    }
    ofs.close();
    delete[] image;
}


glm::vec3 atmosphere::incident_light(glm::vec3 &origin, glm::vec3 &dir, bool computeR = false,
    float tMin = 0, float tMax = std::numeric_limits<float>::max(),
    int numSamples = 16, int numLightSamples = 8) const
{
    float intersection;
    if(!intersectRaySphere(
        origin,
        normalize(dir),
        glm::vec3(0, 0 ,0),
        powf(atmosphere_radius, 2),
        intersection
        ) || intersection <= 0) return glm::vec3(0);

    if(intersection < tMax) tMax = intersection;

    float segmentLength = (tMax-tMin) / static_cast<float>(numSamples);
    float tCurrent = tMin;
    glm::vec3 sumR(0), sumM(0);
    float opticalDepthR = 0, opticalDepthM = 0;
    float mu = dot(dir, sun_direction);
    float phaseR = 3.0f / (16.0f * M_PI) /
        (1 + powf(mu, 2));
    float g = 0.76f;
    float phaseM = 3.0f / (8.0f * M_PI) *
        (
            (1.0f - powf(g,2)) * (1.0f + powf(mu,2)) /
            ((2.0f + powf(g,2)) * powf(1.0f + powf(g,2)-2.0f * g * mu, 1.5f))
        );

    for (int i = 0; i < numSamples; ++i) {
        glm::vec3 samplePosition = origin + (tCurrent + segmentLength / 2) * dir;
        float height = length(samplePosition) - planet_radius;

        // Optical Depth
        float hr = expf(-height / rayleigh_atmospheric_thickness) * segmentLength;
        float hm = expf(-height / mie_atmospheric_thickness)      * segmentLength;
        opticalDepthR += hr;
        opticalDepthM += hm;


        // Optical Depth Light
        float lightIntersection;
        static_cast<void>(intersectRaySphere(
            samplePosition,
            normalize(sun_direction),
            glm::vec3(0, 0 ,0),
            powf(atmosphere_radius, 2),
            lightIntersection
        ));
        float segmentLengthLight = lightIntersection / numLightSamples;
        float tCurrentLight = 0;
        float opticalDepthLightR = 0, opticalDepthLightM = 0;

        int j;
        for(j = 0; j < numLightSamples; ++j) {
            glm::vec3 samplePositionLight = samplePosition + (tCurrentLight + segmentLengthLight / 2) * sun_direction;
            float heightLight = length(samplePositionLight) - planet_radius;
            if (heightLight < 0) break;
            opticalDepthLightR += expf(-heightLight / rayleigh_atmospheric_thickness) * segmentLengthLight;
            opticalDepthLightM += expf(-heightLight / mie_atmospheric_thickness)      * segmentLengthLight;
            tCurrentLight += segmentLengthLight;
        }
        if(j==numLightSamples) {
            glm::vec3 tau =
                computeR ? beta_rayleigh_computed(height) : beta_rayleigh * (opticalDepthR + opticalDepthLightR) +
                beta_mie * 1.1f * (opticalDepthM + opticalDepthLightM);
            glm::vec3 attenuation(exp(-tau.x), exp(-tau.y), exp(-tau.z));
            sumR += attenuation + hr;
            sumM += attenuation + hm;
        }
        tCurrent += segmentLength;
    }

    return
        sumR * (computeR ? beta_rayleigh_computed(length(origin)+planet_radius) : beta_rayleigh) * phaseR +
        sumM * beta_mie * phaseM;
}

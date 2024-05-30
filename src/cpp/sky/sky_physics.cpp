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
#include "glm/detail/_noise.hpp"
#include "glm/detail/_noise.hpp"
#include "glm/ext/quaternion_exponential.hpp"
#include "glm/gtc/constants.hpp"

#define GLM_ENABLE_EXPERIMENTAL
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

glm::vec3 atmosphere::incident_light(const glm::vec3& origin, const glm::vec3 &dir, bool computeR,
    float tMin, float tMax,
    int numSamples, int numLightSamples) const
{
    float intersection;
    if(!intersectRaySphere(
        origin,
        normalize(dir),
        glm::vec3(0, 0 ,0),
        powf(atmosphere_radius, 2),
        intersection
        ) || intersection < 0) return glm::vec3(0);

    if(intersection < tMax) tMax = intersection;

    float segmentLength = (tMax-tMin) / static_cast<float>(numSamples);
    float tCurrent = tMin;
    glm::vec3 sumR(0), sumM(0);
    float opticalDepthR = 0, opticalDepthM = 0;
    float mu = dot(dir, sun_direction);
    float phaseR = 3.0f / (16.0f * M_PI) * (1 + powf(mu, 2));
    float g = 0.76f;
    float phaseM = 3.0f / (8.0f * M_PI) *
        (
            (1.0f - powf(g,2)) * (1.0f + powf(mu,2)) /
            ((2.0f + powf(g,2)) * powf(1.0f + powf(g,2) - 2.0f * g * mu, 1.5f))
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
        if(j == numLightSamples) {
            glm::vec3 tau =
                (computeR ? beta_rayleigh_computed(height) : beta_rayleigh) * (opticalDepthR + opticalDepthLightR) +
                beta_mie * 1.1f * (opticalDepthM + opticalDepthLightM);
            glm::vec3 attenuation(exp(-tau.x), exp(-tau.y), exp(-tau.z));
            sumR += attenuation * hr;
            sumM += attenuation * hm;
        }
        tCurrent += segmentLength;
    }

    return (
            sumR * (computeR ? beta_rayleigh_computed(length(origin)+planet_radius) : beta_rayleigh) * phaseR +
            sumM * beta_mie * phaseM
    ) * 20.0f;
}

#ifndef MAIN_SKY_PHYSICS_H
#define MAIN_SKY_PHYSICS_H

#include "glm/fwd.hpp"
#include "glm/vec3.hpp"

class atmosphere {
public:
    /// Defaults are pertinent to the earth.
    explicit atmosphere(
        glm::vec3 sunDirection = glm::vec3(0,1,0),
        float planetRadius = 6371e3,
        float atmosphereRadius = 6371e4,
        float rayleighAtmosphericThickness = 7994,
        float mieAtmosphericThickness = 1200,
        glm::vec3 betaRayleigh = glm::vec3(3.8e-6f, 13.5e-6f, 33.1e-6f),
        glm::vec3 betaMie = glm::vec3(21e-6f),
        float molecularDensity = 1.2f,// kg/m^3
        float airRefractionIndex = 1.0003f,
        float rayleighScaleHeight = 8.0e3f,
        float mieScaleHeight = 1.2e3f
        ) :
    sun_direction(sunDirection),
    planet_radius(planetRadius),
    atmosphere_radius(atmosphereRadius),
    rayleigh_atmospheric_thickness(rayleighAtmosphericThickness),
    mie_atmospheric_thickness(mieAtmosphericThickness),
    beta_rayleigh(betaRayleigh),
    beta_mie(betaMie),
    molecular_density(molecularDensity),
    air_refraction_index(airRefractionIndex),
    rayleigh_scale_height(rayleighScaleHeight),
    mie_scale_height(mieScaleHeight)
    {}


    /// Direction from which the sun is facing
    glm::vec3 sun_direction;
    /// Radius of the planet.
    float planet_radius;
    /// Radius of the atmosphere around the planet.
    float atmosphere_radius;
    /// Atmospheric thickness for Rayleigh scattering (Hr). Assumes density as uniform.
    float rayleigh_atmospheric_thickness;
    /// Atmospheric thickness for Mie scattering (Hm). Assumes density as uniform.
    float mie_atmospheric_thickness;
    /// Rayleigh scattering coefficient (βr). Assumes sea level.
    glm::vec3 beta_rayleigh;
    /// Mie scattering coefficient (βm). Assumes sea level.
    glm::vec3 beta_mie;
    /// Assumes sea level. Measured in kg/m^3.
    float molecular_density;
    /// Assumes sea level.
    float air_refraction_index;
    /// "A general way to describe how a value fades away".
    float rayleigh_scale_height;
    /// "A general way to describe how a value fades away".
    float mie_scale_height;


    /// Rayleigh scattering coefficient (βr).
    /// @param height Height above sea level.
    /// @param wavelength
    /// @return Scattering coefficient (βr).
    glm::vec3 beta_rayleigh_computed(float height) const;

    /// Cosine smilarity between two angles.
    static float cos_sim(glm::vec3 a, glm::vec3 b);


    float rayleigh_extinction_coefficient(double height, double wavelength) const;

    /// @return Raleigh phase coefficient value.
    float rayleigh_phase(glm::vec3 view_direction) const;

    glm::vec3 wavelength_peak = glm::vec3(
        680e-6f,    // Red
        550e-6f,    // Blue
        440e-6f     // Green
        );

    float mie_extinction_coefficient();

    /// @param light_direction Incoming light direction.
    /// @param view_direction View direction from camera.
    /// @return Raleigh phase value.
    float mie_phase_function(glm::vec3 light_direction, glm::vec3 view_direction) const;

    glm::vec3 incident_light(glm::vec3 & origin, glm::vec3 & dir, bool computeR, float tMin, float tMax, int numSamples, int numLightSamples) const;

    static void render_skydome(glm::vec3 sunDir, const char *filename);
};

#endif //MAIN_SKY_PHYSICS_H

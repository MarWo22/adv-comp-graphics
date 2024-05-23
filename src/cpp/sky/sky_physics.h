#ifndef MAIN_SKY_PHYSICS_H
#define MAIN_SKY_PHYSICS_H
#include <map>

#include "glm/fwd.hpp"
#include "glm/vec3.hpp"

class atmosphere {
public:

    atmosphere(
        glm::vec3 sunDirection = glm::vec3(0,1,0),
        float earthRadius = 6371e3, // https://en.wikipedia.org/wiki/Earth
        float atmosphereRadius = 6371e4 //
        // TODO Finish this
        ) :
    sun_direction(sunDirection)
    {}

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// GENERAL
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /// Direction from which the sun is facing
    glm::vec3 sun_direction;
    /// Radius of the planet.
    float planet_radius = 6371e3;
    /// Radius of the atmosphere around the planet.
    float atmosphere_radius = 6471e3;
    /// Atmospheric thickness for Rayleigh scattering (Hr). Assumes density as uniform.
    float rayleigh_atmospheric_thickness = 1.0;
    /// Atmospheric thickness for Mie scattering (Hm). Assumes density as uniform.
    float mie_atmospheric_thickness = 1.0;

    /// @param height Height above sea level.
    /// @return TODO
    double molecular_density(double height) const;

    static double cos_similarity(glm::vec3 vec, glm::vec3 vec3);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// RAYLEIGH
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /// "A general way to describe how a value fades away". In kilometers.
    double rayleigh_scale_height = 8.0;

    /// Rayleigh β-scattering coefficient.
    /// @param height Height above sea level.
    /// @param wavelength
    /// @return Scattering coefficient (β).
    double rayleigh_scattering_coefficient(double height, double wavelength) const;

    double rayleigh_extinction_coefficient(double height, double wavelength) const;

    /// @param light_direction Incoming light direction.
    /// @param view_direction View direction from camera.
    /// @return Raleigh phase coefficient value.
    double rayleigh_phase_function(glm::vec3 light_direction, glm::vec3 view_direction) const;

    /// Precomputed Rayleigh coefficient values and their respective wavelengths. Keys in nanometers.
    std::map<double, double> scatter_coefficients = {
        {440, 33.1e-6}, // Blue
        {550, 13.5e-6}, // Green
        {680, 5.8e-6}   // Red
    };

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// MIE
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    double mie_scale_height = 1.2;

    double mie_scattering_coefficient = 210e-5;

    double mie_extinction_coefficient = mie_scattering_coefficient * 1.1;

    /// @param light_direction Incoming light direction.
    /// @param view_direction View direction from camera.
    /// @return Raleigh phase value.
    double mie_phase_function(glm::vec3 light_direction, glm::vec3 view_direction) const;

}; // sky

#endif //MAIN_SKY_PHYSICS_H

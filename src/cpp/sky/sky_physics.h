#ifndef MAIN_SKY_PHYSICS_H
#define MAIN_SKY_PHYSICS_H
#include <map>
#include "glm/vec3.hpp"

class sky {
public:
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// GENERAL
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /// In kilometers. https://en.wikipedia.org/wiki/Earth
    double earth_radius = 6371.0;
    /// In kilometers. https://en.wikipedia.org/wiki/Atmosphere_of_Earth
    double atmosphere_radius = earth_radius * 1.0157;
    double atmospheric_thickness = 1.0;
    double sea_level_molecular_density = 1.0;
    double medium_anisotropy = 0.76;

    /// "A general way to describe how a value fades away". In kilometers.
    int air_refraction_index = 1;

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

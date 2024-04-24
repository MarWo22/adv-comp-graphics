#ifndef MAIN_SKY_PHYSICS_H
#define MAIN_SKY_PHYSICS_H
#include <map>
#include "glm/vec3.hpp"

class sky {
public:
    /// In kilometers. https://en.wikipedia.org/wiki/Earth
    double earth_radius = 6371.0;
    /// In kilometers. https://en.wikipedia.org/wiki/Atmosphere_of_Earth
    double atmosphere_radius = earth_radius * 1.0157;
    double atmospheric_thickness = 1.0;
    double sea_level_molecular_density = 1.0;
    /// "A general way to describe how a value fades away". In kilometers.
    double scale_height = 8.0;
    int refraction_index = 1;

    /// @param height Height above sea level.
    /// @return TODO
    double density(double height) const;

    /// Rayleigh β-scattering coefficient.
    /// @param height Height above sea level.
    /// @return Scattering coefficient (β).
    double raleigh_scattering_coefficient(double height) const;

    /// 
    /// @param light_direction Incoming light direction.
    /// @param view_direction View direction from camera.
    /// @return Raleigh phase value.
    static double sky::raleigh_phase_function(glm::vec3 light_direction, glm::vec3 view_direction);

    /// 
    /// @param µ Cosine between light and view directions.
    /// @return Raleigh phase value.
    static double sky::raleigh_phase_function(double µ);

    /// Precomputed Rayleigh coefficient values and their respective wavelengths. Keys in nanometers.
    std::map<double, double> scatter_coefficients = {
        {440, 33.1e-6}, // Blue
        {550, 13.5e-6}, // Green
        {680, 5.8e-6}   // Red
    };

}; // sky

#endif //MAIN_SKY_PHYSICS_H

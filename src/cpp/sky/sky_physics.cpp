#include "sky_physics.h"
#include "cmath"
#include "glm/vec3.hpp"
#include "glm/gtx/vector_angle.inl"

double atmosphere::molecular_density(const double height) const {
    return sea_level_molecular_density * exp(-height / atmospheric_thickness);
}

double atmosphere::rayleigh_scattering_coefficient(const double height, const double wavelength) const {
    return
    8 * pow(M_PI, 3) * pow(pow(air_refraction_index, 2) - 1, 2) /
    (3 * sea_level_molecular_density * pow(wavelength, 4)) *
    exp(-height/rayleigh_scale_height);
}
double atmosphere::rayleigh_extinction_coefficient(const double height, const double wavelength) const {
    return rayleigh_scattering_coefficient(height, wavelength);
}

double atmosphere::rayleigh_phase_function(const glm::vec3 light_direction, const glm::vec3 view_direction) const {
    return 3 / (16 * M_PI) * (1 + pow(cos_similarity(light_direction, view_direction), 2));
}

double atmosphere::mie_phase_function(const glm::vec3 light_direction, const glm::vec3 view_direction) const {
    const double my = cos_similarity(light_direction, view_direction);
    return
    3 / (8 * M_PI) *
    ((1-pow(medium_anisotropy, 2)) * (1 + pow(my, 2))) /
    ((2+pow(medium_anisotropy, 2))*pow(1 + pow(medium_anisotropy, 2) - 2 * medium_anisotropy * my, 3.0/2.0)) ;
}

double atmosphere::cos_similarity(const glm::vec3 light_direction, const glm::vec3 view_direction) {
    return cos(angle(light_direction, view_direction));
}

//TODO Optical Depth
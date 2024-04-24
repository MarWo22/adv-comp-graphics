#include "sky_physics.h"
#include "cmath"
#include "glm/vec3.hpp"
#include "glm/gtx/vector_angle.inl"

double sky::density(double height) const {
    return sea_level_molecular_density * exp(-height / atmospheric_thickness);
}

double sky::raleigh_scattering_coefficient(const double height) const{
    return
    (8 * pow(M_PI, 2) * pow((pow(refraction_index, 2) - 1),2)) /
    (3 * sea_level_molecular_density) *
    exp(-height/scale_height);
}

double sky::raleigh_phase_function(const glm::vec3 light_direction, const glm::vec3 view_direction) {
    const double my = cos(glm::angle(light_direction, view_direction));
    return raleigh_phase_function(my);
}

double sky::raleigh_phase_function(const double µ) {
    return 3/16*M_PI(1+pow(µ,2));
}


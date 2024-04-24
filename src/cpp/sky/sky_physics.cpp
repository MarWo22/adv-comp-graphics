#include "sky_physics.h"
#include "cmath"

double Sky::Density(double height) const {
    return SeaLevelMolecularDensity * exp(-height / AtmosphericThickness);
}

double Sky::Rayleigh(double height, double wavelength) const{

    return
    (8 * pow(M_PI, 2) * pow((pow(RefractionIndex, 2) - 1),2)) /
    (3 * SeaLevelMolecularDensity) *
    exp(-height/ScaleHeight);
}

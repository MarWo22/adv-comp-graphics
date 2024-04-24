#ifndef MAIN_SKY_PHYSICS_H
#define MAIN_SKY_PHYSICS_H

class Sky {
public:
    /// In kilometers. https://en.wikipedia.org/wiki/Earth
    double EarthRadius = 6371.0;
    /// In kilometers. https://en.wikipedia.org/wiki/Atmosphere_of_Earth
    double atmosphereRadius = EarthRadius * 1.0157;
    double AtmosphericThickness = 1.0;
    double SeaLevelMolecularDensity = 1.0;
    /// In kilometers. Paper's default is 8.
    double ScaleHeight = 8.0;
    int RefractionIndex = 1;

    /// Calculates density at a given height.
    double Density(double height) const;
    /// Rayleigh β-scattering coefficient.
    double Rayleigh(double height, double wavelength) const;


}; // sky

#endif //MAIN_SKY_PHYSICS_H

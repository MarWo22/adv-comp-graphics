//
// Created by marti on 23/05/2024.
//

#ifndef MAIN_DENSITY_H
#define MAIN_DENSITY_H


#include "../../shader.h"

class Density
{
    Shader *m_DensityShader;
    GLuint m_DensityTexture;
public:
    Density() = default;
    explicit Density(Shader *densityShader);
    void CalculateDensity(const glm::ivec3 &pos, int size);
    void SetupIteration();

private:
    void GenerateTexture();

};


#endif //MAIN_DENSITY_H

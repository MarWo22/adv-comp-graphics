//
// Created by marti on 23/05/2024.
//

#include <iostream>
#include "density.h"

Density::Density(Shader *densityShader)
    : m_DensityShader(densityShader)
    , m_DensityTexture(0)
{
    GenerateTexture();
}

void Density::GenerateTexture()
{
    glGenTextures(1, &m_DensityTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, m_DensityTexture);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glTexImage3D(GL_TEXTURE_3D, 0, GL_R32F, 33, 33, 33, 0, GL_RED, GL_FLOAT, NULL);
    glBindImageTexture(0, m_DensityTexture, 0, GL_TRUE, 0, GL_READ_WRITE, GL_R32F);
}

void Density::SetupIteration()
{
    m_DensityShader->Bind();
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_3D, m_DensityTexture );
}

void Density::CalculateDensity(const glm::ivec3 &pos, int size)
{
    m_DensityShader->Bind();
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_3D, m_DensityTexture );

    m_DensityShader->SetUniform("block_position", pos);
    m_DensityShader->SetUniform("block_size", static_cast<float>(size));

    glDispatchCompute(5, 5, 5);
    glMemoryBarrier( GL_SHADER_IMAGE_ACCESS_BARRIER_BIT );
}

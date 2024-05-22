#include <iostream>
#include "texture.h"
#include "stb_image.h"
#include "image.h"

Texture::Texture(const std::string &filename)
{
    Load(filename);
}


void Texture::Load(const std::string& filename)
{
    glGenTextures(1, &id_);
    glBindTexture(GL_TEXTURE_2D, id_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    Image image(filename);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, image.width, image.height, 0, GL_RGB, GL_FLOAT, image.data);
}

void Texture::Bind(GLenum texture_unit) const
{
    glActiveTexture(texture_unit);
    glBindTexture(GL_TEXTURE_2D, id_);
}
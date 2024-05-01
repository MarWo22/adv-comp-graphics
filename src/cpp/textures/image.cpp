#include <string>
#include <iostream>
#include "image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Image::Image(const std::string& filename)
{
    stbi_set_flip_vertically_on_load(true);
    data = stbi_loadf(filename.c_str(), &width, &height, &components, 3);
    if(data == nullptr)
    {
        std::cout << "Failed to load image: " << filename << ".\n";
        exit(1);
    }
}

Image::~Image()
{
    stbi_image_free(data);
};

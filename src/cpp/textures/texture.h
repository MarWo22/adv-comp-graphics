#ifndef TEXTURE_H_
#define TEXTURE_H_


#include <string>
#include "GL/glew.h"

class Texture
{
private:
    GLuint id_ = 0;

public:

    Texture() = default;

    explicit Texture(const std::string &filename);

    void Load(const std::string &filename);

    inline GLuint id() const { return id_; };

    void Bind(GLenum texture_unit) const;

};


#endif //TEXTURE_H_

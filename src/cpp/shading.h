#ifndef SHADING_H
#define SHADING_H

#include <string>
#include <GL/glew.h>

namespace shading
{
    GLuint loadShaderProgram(const std::string& vertexShader, const std::string& fragmentShader, bool allow_errors);

    std::string GetShaderInfoLog(GLuint obj);

    bool linkShaderProgram(GLuint shaderProgram, bool allow_errors);

    std::string GetShaderProgramInfoLog(GLuint obj);

    void setUniform(GLint uniformLocation, uint32_t nof_values, const glm::vec3 *values);

    void setUniform(GLint uniformLocation, const glm::vec3 &value);

    void setUniform(GLint uniformLocation, bool value);

    void setUniform(GLint uniformLocation, GLuint value);

    void setUniform(GLint uniformLocation, GLint value);

    void setUniform(GLint uniformLocation, float value);

    void setUniform(GLint uniformLocation, const glm::mat4 &matrix);
}


#endif //SHADING_H

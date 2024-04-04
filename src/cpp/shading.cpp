#include <fstream>
#include <iostream>
#include <glm/glm.hpp>
#include "shading.h"

#include "gl_misc.h"


std::string shading::GetShaderInfoLog(GLuint obj)
{
    int logLength = 0;
    int charsWritten = 0;
    char* tmpLog;
    std::string log;

    glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &logLength);

    if(logLength > 0)
    {
        tmpLog = new char[logLength];
        glGetShaderInfoLog(obj, logLength, &charsWritten, tmpLog);
        log = tmpLog;
        delete[] tmpLog;
    }

    return log;
}

GLuint shading::loadShaderProgram(const std::string& vertexShader, const std::string& fragmentShader, bool allow_errors)
{
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    std::ifstream vs_file(vertexShader);
    std::string vs_src((std::istreambuf_iterator<char>(vs_file)), std::istreambuf_iterator<char>());

    std::ifstream fs_file(fragmentShader);
    std::string fs_src((std::istreambuf_iterator<char>(fs_file)), std::istreambuf_iterator<char>());

    const char* vs = vs_src.c_str();
    const char* fs = fs_src.c_str();

    glShaderSource(vShader, 1, &vs, nullptr);
    glShaderSource(fShader, 1, &fs, nullptr);
    // text data is not needed beyond this point
ion
    glCompileShader(vShader);
    int compileOk = 0;
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &compileOk);
    if(!compileOk)
    {
        std::string err = GetShaderInfoLog(vShader);
        if(allow_errors)
        {
            gl_misc::non_fatal_error(err, "Vertex Shader");
        }
        else
        {
            gl_misc::fatal_error(err, "Vertex Shader");
        }
        return 0;
    }

    glCompileShader(fShader);
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &compileOk);
    if(!compileOk)
    {
        std::string err = GetShaderInfoLog(fShader);
        if(allow_errors)
        {
            gl_misc::non_fatal_error(err, "Fragment Shader");
        }
        else
        {
            gl_misc::fatal_error(err, "Fragment Shader");
        }
        return 0;
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, fShader);
    glDeleteShader(fShader);
    glAttachShader(shaderProgram, vShader);
    glDeleteShader(vShader);
    if(!allow_errors)
        CHECK_GL_ERROR();

    if(!linkShaderProgram(shaderProgram, allow_errors))
        return 0;

    return shaderProgram;
}

bool shading::linkShaderProgram(GLuint shaderProgram, bool allow_errors)
{
    glLinkProgram(shaderProgram);
    GLint linkOk = 0;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkOk);
    if(!linkOk)
    {
        std::string err = GetShaderProgramInfoLog(shaderProgram);
        if(allow_errors)
        {
            gl_misc::non_fatal_error(err, "Linking");
        }
        else
        {
            gl_misc::fatal_error(err, "Linking");
        }
        return false;
    }
    return true;
}

std::string shading::GetShaderProgramInfoLog(GLuint obj)
{
    int logLength = 0;
    int charsWritten = 0;
    char* tmpLog;
    std::string log;

    glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &logLength);

    if(logLength > 0)
    {
        tmpLog = new char[logLength];
        glGetProgramInfoLog(obj, logLength, &charsWritten, tmpLog);
        log = tmpLog;
        delete[] tmpLog;
    }

    return log;
}

void shading::setUniform(GLint uniformLocation, const glm::mat4& matrix)
{
    glUniformMatrix4fv(uniformLocation, 1, false, &matrix[0].x);
}
void shading::setUniform(GLint uniformLocation, float value)
{
    glUniform1f(uniformLocation, value);
}
void shading::setUniform(GLint uniformLocation, GLint value)
{
    glUniform1i(uniformLocation, value);
}
void shading::setUniform(GLint uniformLocation, GLuint value)
{
    glUniform1ui(uniformLocation, value);
}
void shading::setUniform(GLint uniformLocation, bool value)
{
    glUniform1i(uniformLocation, value ? 1 : 0);
}
void shading::setUniform(GLint uniformLocation, const glm::vec3& value)
{
    glUniform3fv(uniformLocation, 1, &value.x);
}
void shading::setUniform(GLint uniformLocation, const uint32_t nof_values, const glm::vec3* values)
{
    glUniform3fv(uniformLocation, nof_values, (float*)values);
}

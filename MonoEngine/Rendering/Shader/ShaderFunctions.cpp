
#include "ShaderFunctions.h"
#include "IShaderFactory.h"
#include "ShaderFactory.h"

#include "System/open_gl.h"

#include <vector>
#include <string>

namespace
{
    const mono::IShaderFactory* s_shader_factory = nullptr;
}

size_t mono::CompileShader(mono::ShaderType type, const char* source)
{
    const GLenum shaderType = (type == mono::ShaderType::VERTEX) ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
    const GLuint shader = glCreateShader(shaderType);
    if(shader == 0)
        throw std::runtime_error("Unable to create shader: " + std::string(source));

    glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);

    GLint compiledSuccessfully = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiledSuccessfully);
    if(compiledSuccessfully != GL_TRUE)
    {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        GLint outLength = 0;
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &outLength, infoLog.data());

        // We don't need the shader anymore.
        glDeleteShader(shader);

        // Store the compile log
        const std::string log(infoLog.begin(), infoLog.end());

        // Throw here since we could not compile the shader
        throw std::runtime_error("Unable to compile shader: " + std::string(source) + "\nLog: " + log);
    }

    return shader;
}

size_t mono::LinkProgram(size_t vertexShader, size_t fragmentShader)
{
    const GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint linkedSuccessfully = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linkedSuccessfully);
    if(linkedSuccessfully != GL_TRUE)
    {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        GLint outLength = 0;
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &outLength, infoLog.data());

        // We don't need the program anymore.
        glDeleteProgram(program);

        // Don't leak shaders either.
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // Store the link log
        const std::string log(infoLog.begin(), infoLog.end());

        // Throw here since we could not link the shader
        throw std::runtime_error("Unable to link program. - " + log);
    }

    // Always detach shaders after a successful link.
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);

    return program;
}

void mono::LoadDefaultShaderFactory()
{
    UnloadShaderFactory();
    s_shader_factory = new mono::ShaderFactory;
}

void mono::LoadCustomShaderFactory(const mono::IShaderFactory* custom_factory)
{
    UnloadShaderFactory();
    s_shader_factory = custom_factory;
}

const mono::IShaderFactory* mono::GetShaderFactory()
{
    return s_shader_factory;
}

void mono::UnloadShaderFactory()
{
    if(s_shader_factory)
        delete s_shader_factory;

    s_shader_factory = nullptr;
}


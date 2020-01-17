
#include "ShaderFunctions.h"
#include "IShaderFactory.h"

#include "System/open_gl.h"

#include <vector>
#include <string>

// Initialized in RenderSystem.cpp
extern mono::IShaderFactory* g_shader_factory;

uint32_t mono::CompileShader(mono::ShaderType type, const char* source)
{
    const GLenum shader_type = (type == mono::ShaderType::VERTEX) ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
    const GLuint shader = glCreateShader(shader_type);
    if(shader == 0)
        throw std::runtime_error("Unable to create shader: " + std::string(source));

    glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);

    GLint compiled_successfully = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled_successfully);
    if(compiled_successfully != GL_TRUE)
    {
        GLint max_length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);

        GLint out_length = 0;
        std::vector<GLchar> info_log(max_length);
        glGetShaderInfoLog(shader, max_length, &out_length, info_log.data());

        glDeleteShader(shader);

        const std::string log(info_log.begin(), info_log.end());
        throw std::runtime_error("Unable to compile shader: " + std::string(source) + "\nLog: " + log);
    }

    return shader;
}

uint32_t mono::LinkProgram(uint32_t vertex_shader, uint32_t fragment_shader)
{
    const GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    GLint linked_successfully = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linked_successfully);
    if(linked_successfully != GL_TRUE)
    {
        GLint max_length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);

        GLint out_length = 0;
        std::vector<GLchar> info_log(max_length);
        glGetProgramInfoLog(program, max_length, &out_length, info_log.data());

        glDeleteProgram(program);

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        const std::string log(info_log.begin(), info_log.end());
        throw std::runtime_error("Unable to link program. - " + log);
    }

    // Always detach shaders after a successful link.
    glDetachShader(program, vertex_shader);
    glDetachShader(program, fragment_shader);

    return program;
}

const mono::IShaderFactory* mono::GetShaderFactory()
{
    return g_shader_factory;
}

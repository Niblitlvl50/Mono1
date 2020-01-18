
#include "Shader.h"
#include "ShaderFunctions.h"
#include "Math/Matrix.h"
#include "Math/Vector.h"
#include "Util/Hash.h"
#include "Rendering/Color.h"

#include "System/open_gl.h"

using namespace mono;

namespace
{
    int GetUniformLocation(uint32_t program, const char* location_name, std::unordered_map<uint32_t, int>& location_cache)
    {
        const uint32_t name_hash = mono::Hash(location_name);
        const auto it = location_cache.find(name_hash);
        if(it != location_cache.end())
            return it->second;

        const int program_location = glGetUniformLocation(program, location_name);
        location_cache[name_hash] = program_location;
        return program_location;
    }

    int GetAttribLocation(uint32_t program, const char* location_name, std::unordered_map<uint32_t, int>& location_cache)
    {
        const uint32_t name_hash = mono::Hash(location_name);
        const auto it = location_cache.find(name_hash);
        if(it != location_cache.end())
            return it->second;

        const int program_location = glGetAttribLocation(program, location_name);
        location_cache[name_hash] = program_location;
        return program_location;
    }
}

Shader::Shader(const char* vertex_source, const char* fragment_source)
{
    const GLuint vertex_shader = CompileShader(mono::ShaderType::VERTEX, vertex_source);
    const GLuint fragment_shader = CompileShader(mono::ShaderType::FRAGMENT, fragment_source);
    m_program = LinkProgram(vertex_shader, fragment_shader);
}

Shader::~Shader()
{
    // delete shader, delete program?
    glDeleteProgram(m_program);
}

void Shader::Use()
{
    glUseProgram(m_program);
}

uint32_t Shader::GetShaderId() const
{
    return m_program;
}

int Shader::GetAttributeLocation(const char* attribute_name)
{
    return GetAttribLocation(m_program, attribute_name, m_location_cache);
}

void Shader::SetValue(const char* property_name, int value)
{
    const int uniform_location = GetUniformLocation(m_program, property_name, m_location_cache);
    glUniform1i(uniform_location, value);
}

void Shader::SetValue(const char* property_name, float value)
{
    const int uniform_location = GetUniformLocation(m_program, property_name, m_location_cache);
    glUniform1f(uniform_location, value);
}

void Shader::SetValue(const char* property_name, const math::Vector& vector)
{
    const int uniform_location = GetUniformLocation(m_program, property_name, m_location_cache);
    glUniform2f(uniform_location, vector.x, vector.y);
}

void Shader::SetValue(const char* property_name, const math::Matrix& transform)
{
    const int uniform_location = GetUniformLocation(m_program, property_name, m_location_cache);
    glUniformMatrix4fv(uniform_location, 1, GL_FALSE, transform.data);
}

void Shader::SetValue(const char* property_name, const mono::Color::RGBA& color)
{
    const int uniform_location = GetUniformLocation(m_program, property_name, m_location_cache);
    glUniform4f(uniform_location, color.red, color.green, color.blue, color.alpha);
}

void Shader::SetProjectionAndModelView(const math::Matrix& projection, const math::Matrix& model_view)
{
    SetValue("p_matrix", projection);
    SetValue("mv_matrix", model_view);
}

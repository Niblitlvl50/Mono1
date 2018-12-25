
#include "PointSpriteShader.h"
#include "ShaderFunctions.h"
#include "Math/Matrix.h"

#include "System/open_gl.h"

namespace
{
    constexpr const char* vertex_source = R"(

    #ifdef __IPHONEOS__
        precision mediump float;
    #endif

        attribute vec2 vertex_position;
        attribute float vertex_rotation;
        attribute vec4 vertex_color;
        attribute float point_size;

        uniform mat4 mv_matrix;
        uniform mat4 p_matrix;

        varying vec4 color;
        varying float rotation;

        void main()
        {
            gl_Position = p_matrix * mv_matrix * vec4(vertex_position, 0.0, 1.0);
            gl_PointSize = point_size;
            color = vertex_color;
            rotation = vertex_rotation;
        }
    )";

    constexpr const char* fragment_source = R"(

        #version 120

    #ifdef __IPHONEOS__
        precision mediump float;
    #endif

        varying vec4 color;
        varying float rotation;
        uniform sampler2D sampler;

        void main()
        {
            vec2 coord = gl_PointCoord;
            float sin_factor = sin(rotation);
            float cos_factor = cos(rotation);
            coord = (coord - 0.5) * mat2(cos_factor, sin_factor, -sin_factor, cos_factor);
            coord += 0.5;

            gl_FragColor = texture2D(sampler, coord) * color;
        }    
    )";
}


using namespace mono;

PointSpriteShader::PointSpriteShader()
{
    const GLuint vertex_shader = CompileShader(mono::ShaderType::VERTEX, vertex_source);
    const GLuint fragment_shader = CompileShader(mono::ShaderType::FRAGMENT, fragment_source);

    m_program = LinkProgram(vertex_shader, fragment_shader);

    m_mv_matrix_location = glGetUniformLocation(m_program, "mv_matrix");
    m_p_matrix_location = glGetUniformLocation(m_program, "p_matrix");

    m_position_attribute = (unsigned int)glGetAttribLocation(m_program, "vertex_position");
    m_rotation_attribute = (unsigned int)glGetAttribLocation(m_program, "vertex_rotation");
    m_color_attribute = (unsigned int)glGetAttribLocation(m_program, "vertex_color");
    m_point_size_attribute = (unsigned int)glGetAttribLocation(m_program, "point_size");
}

void PointSpriteShader::Use()
{
    glUseProgram(m_program);
}

unsigned int PointSpriteShader::GetShaderId() const
{
    return m_program;
}

void PointSpriteShader::LoadProjectionMatrix(const math::Matrix& projection)
{
    glUniformMatrix4fv(m_p_matrix_location, 1, GL_FALSE, projection.data);
}

void PointSpriteShader::LoadModelViewMatrix(const math::Matrix& modelView)
{
    glUniformMatrix4fv(m_mv_matrix_location, 1, GL_FALSE, modelView.data);
}

unsigned int PointSpriteShader::GetPositionAttribute() const
{
    return m_position_attribute;
}

unsigned int PointSpriteShader::GetRotationAttribute() const
{
    return m_rotation_attribute;
}

unsigned int PointSpriteShader::GetColorAttribute() const
{
    return m_color_attribute;
}

unsigned int PointSpriteShader::GetPointSizeAttribute() const
{
    return m_point_size_attribute;
}

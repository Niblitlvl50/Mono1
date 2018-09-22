
#include "ColorShader.h"
#include "ShaderFunctions.h"
#include "System/OpenGL.h"
#include "Math/Matrix.h"


namespace
{
    constexpr const char* vertexSource = R"(
    #ifdef __IPHONEOS__
        precision mediump float;
    #endif

        attribute vec2 vertex_position;
        attribute vec4 vertex_color;

        uniform mat4 mv_matrix;
        uniform mat4 p_matrix;
        uniform float point_size;

        varying vec4 color;

        void main()
        {
            gl_Position = p_matrix * mv_matrix * vec4(vertex_position, 0.0, 1.0);
            gl_PointSize = point_size;
            color = vertex_color;
        }
    )";

    constexpr const char* fragmentSource = R"(
    #ifdef __IPHONEOS__
        precision mediump float;
    #endif

        varying vec4 color;

        void main()
        {
            gl_FragColor = color;
        }
    )";
}


using namespace mono;

ColorShader::ColorShader()
{
    const GLuint vertexShader = CompileShader(mono::ShaderType::VERTEX, vertexSource);
    const GLuint fragmentShader = CompileShader(mono::ShaderType::FRAGMENT, fragmentSource);

    m_program = LinkProgram(vertexShader, fragmentShader);

    m_mv_matrix_location = glGetUniformLocation(m_program, "mv_matrix");
    m_p_matrix_location = glGetUniformLocation(m_program, "p_matrix");
    m_point_size_location = glGetUniformLocation(m_program, "point_size");

    m_position_attribute_location = (unsigned int)glGetAttribLocation(m_program, "vertex_position");
    m_color_attribute_location = (unsigned int)glGetAttribLocation(m_program, "vertex_color");
}

void ColorShader::Use()
{
    glUseProgram(m_program);
}

unsigned int ColorShader::GetShaderId() const
{
    return m_program;
}

void ColorShader::LoadProjectionMatrix(const math::Matrix& projection)
{
    glUniformMatrix4fv(m_p_matrix_location, 1, GL_FALSE, projection.data);
}

void ColorShader::LoadModelViewMatrix(const math::Matrix& modelView)
{
    glUniformMatrix4fv(m_mv_matrix_location, 1, GL_FALSE, modelView.data);
}

void ColorShader::SetPointSize(float size)
{
    glUniform1f(m_point_size_location, size);
}

unsigned int ColorShader::GetPositionAttributeLocation() const
{
    return m_position_attribute_location;
}

unsigned int ColorShader::GetColorAttributeLocation() const
{
    return m_color_attribute_location;
}

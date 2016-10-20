//
//  ImGuiShader.cpp
//  MonoiOS
//
//  Created by Niklas Damberg on 19/06/16.
//
//

#include "ImGuiShader.h"
#include "Shader/ShaderFunctions.h"
#include "System/SysOpenGL.h"
#include "Math/Matrix.h"

namespace
{
    constexpr const char* vertex_source =

#ifdef __IPHONEOS__
    "precision mediump float;"
#endif

    "uniform mat4 mv_matrix;"
    "uniform mat4 p_matrix;"

    "attribute vec2 Position;"
    "attribute vec2 UV;"
    "attribute vec4 Color;"

    "varying vec2 Frag_UV;"
    "varying vec4 Frag_Color;"

    "void main()"
    "{"
    "   Frag_UV = UV;"
    "   Frag_Color = Color;"
    "   gl_Position = p_matrix * mv_matrix * vec4(Position.xy, 0.0, 1.0);"
    "}";

    constexpr const char* fragment_source =

#ifdef __IPHONEOS__
    "precision mediump float;"
#endif

    "uniform sampler2D Texture;"

    "varying vec2 Frag_UV;"
    "varying vec4 Frag_Color;"

    "void main()"
    "{"
    "   gl_FragColor = Frag_Color * texture2D(Texture, Frag_UV.st);"
    "}";
}

ImGuiShader::ImGuiShader()
{
    const GLuint vertex_shader = CompileShader(mono::ShaderType::VERTEX, vertex_source);
    const GLuint fragment_shader = CompileShader(mono::ShaderType::FRAGMENT, fragment_source);

    m_program = mono::LinkProgram(vertex_shader, fragment_shader);

    m_MVMatrixLocation = glGetUniformLocation(m_program, "mv_matrix");
    m_PMatrixLocation = glGetUniformLocation(m_program, "p_matrix");

    m_textureLocation = glGetUniformLocation(m_program, "Texture");

    m_positionAttributeLocation = (unsigned int)glGetAttribLocation(m_program, "Position");
    m_colorAttributeLocation = (unsigned int)glGetAttribLocation(m_program, "Color");
    m_textureAttributeLocation = (unsigned int)glGetAttribLocation(m_program, "UV");
}

ImGuiShader::~ImGuiShader()
{
    glDeleteProgram(m_program);
}

void ImGuiShader::Use()
{
    glUseProgram(m_program);
}

unsigned int ImGuiShader::GetShaderId() const
{
    return m_program;
}

void ImGuiShader::LoadProjectionMatrix(const math::Matrix& projection)
{
    glUniformMatrix4fv(m_PMatrixLocation, 1, GL_FALSE, projection.data);
}

void ImGuiShader::LoadModelViewMatrix(const math::Matrix& modelView)
{
    glUniformMatrix4fv(m_MVMatrixLocation, 1, GL_FALSE, modelView.data);
}

void ImGuiShader::SetTextureValue(int value)
{
    glUniform1i(m_textureLocation, value);
}

unsigned int ImGuiShader::PositionAttribute() const
{
    return m_positionAttributeLocation;
}

unsigned int ImGuiShader::TextureAttribute() const
{
    return m_textureAttributeLocation;
}

unsigned int ImGuiShader::ColorAttribute() const
{
    return m_colorAttributeLocation;
}

int ImGuiShader::TextureLocation() const
{
    return m_textureLocation;
}


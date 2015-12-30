//
//  MorphingShader.cpp
//  MonoOSX
//
//  Created by Niklas Damberg on 21/12/15.
//
//

#include "MorphingShader.h"
#include "ShaderFunctions.h"
#include "SysOpenGL.h"
#include "Matrix.h"

namespace
{
    constexpr const char* vertexSource =

#ifdef __IPHONEOS__
    "precision mediump float;"
#endif

    "attribute vec2 vertexPosition;"
    "attribute vec2 morphPosition;"
    "attribute vec4 vertexColor;"

    "uniform mat4 mv_matrix;"
    "uniform mat4 p_matrix;"
    "uniform float morphGrade;"

    "varying vec4 color;"

    "void main()"
    "{"
    "    vec2 temp = morphPosition - vertexPosition;"
    "    vec2 final = vertexPosition + (temp * morphGrade);"
    "    gl_Position = p_matrix * mv_matrix * vec4(final, 0.0, 1.0);"
    "    color = vertexColor;"
    "}";

    constexpr const char* fragmentSource =

#ifdef __IPHONEOS__
    "precision mediump float;"
#endif

    "varying vec4 color;"

    "void main()"
    "{"
    "    gl_FragColor = color;"
    "}";
}

using namespace mono;

MorphingShader::MorphingShader()
{
    const GLuint vertexShader = CompileShader(mono::ShaderType::VERTEX, vertexSource);
    const GLuint fragmentShader = CompileShader(mono::ShaderType::FRAGMENT, fragmentSource);

    m_program = LinkProgram(vertexShader, fragmentShader);

    m_MVMatrixLocation = glGetUniformLocation(m_program, "mv_matrix");
    m_PMatrixLocation = glGetUniformLocation(m_program, "p_matrix");
    m_morphGrade = glGetUniformLocation(m_program, "morphGrade");

    m_positionLocation = (unsigned int)glGetAttribLocation(m_program, "vertexPosition");
    m_morphPositionLocation = (unsigned int)glGetAttribLocation(m_program, "morphPosition");
    m_colorLocation = (unsigned int)glGetAttribLocation(m_program, "vertexColor");
}

void MorphingShader::SetMorphGrade(float grade)
{
    glUniform1f(m_morphGrade, grade);
}

unsigned int MorphingShader::GetPositionLocation() const
{
    return m_positionLocation;
}

unsigned int MorphingShader::GetMorphPositionLocation() const
{
    return m_morphPositionLocation;
}

unsigned int MorphingShader::GetColorLocation() const
{
    return m_colorLocation;
}

void MorphingShader::Use()
{
    glUseProgram(m_program);
}

unsigned int MorphingShader::GetShaderId() const
{
    return m_program;
}

void MorphingShader::LoadProjectionMatrix(const math::Matrix& projection)
{
    glUniformMatrix4fv(m_PMatrixLocation, 1, GL_FALSE, projection.data);
}

void MorphingShader::LoadModelViewMatrix(const math::Matrix& modelView)
{
    glUniformMatrix4fv(m_MVMatrixLocation, 1, GL_FALSE, modelView.data);
}

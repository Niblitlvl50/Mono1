//
//  ColorShader.cpp
//  MonoOSX
//
//  Created by Niklas Damberg on 25/12/14.
//
//

#include "ColorShader.h"
#include "ShaderFunctions.h"
#include "SysOpenGL.h"
#include "Matrix.h"


namespace
{
    constexpr const char* vertexSource =

    //"precision mediump float;"
    "attribute vec2 vertexPosition;"
    "attribute vec4 vertexColor;"

    "uniform mat4 mv_matrix;"
    "uniform mat4 p_matrix;"
    "uniform float pointSize;"

    "varying vec4 color;"

    "void main()"
    "{"
    "    gl_Position = p_matrix * mv_matrix * vec4(vertexPosition, 0.0, 1.0);"
    "    gl_PointSize = pointSize;"
    "    color = vertexColor;"
    "}";

    constexpr const char* fragmentSource =

    //"precision mediump float;"
    "varying vec4 color;"

    "void main()"
    "{"
    "    gl_FragColor = color;"
    "}";
}


using namespace mono;

ColorShader::ColorShader()
{
    const GLuint vertexShader = CompileShader(mono::ShaderType::VERTEX, vertexSource);
    const GLuint fragmentShader = CompileShader(mono::ShaderType::FRAGMENT, fragmentSource);

    mProgram = LinkProgram(vertexShader, fragmentShader);

    mMVMatrixLocation = glGetUniformLocation(mProgram, "mv_matrix");
    mPMatrixLocation = glGetUniformLocation(mProgram, "p_matrix");
    m_pointSizeLocation = glGetUniformLocation(mProgram, "pointSize");

    mPositionAttributeLocation = (unsigned int)glGetAttribLocation(mProgram, "vertexPosition");
    mColorAttributeLocation = (unsigned int)glGetAttribLocation(mProgram, "vertexColor");
}

void ColorShader::Use()
{
    glUseProgram(mProgram);
}

unsigned int ColorShader::GetShaderId() const
{
    return mProgram;
}

void ColorShader::LoadProjectionMatrix(const math::Matrix& projection)
{
    glUniformMatrix4fv(mPMatrixLocation, 1, GL_FALSE, projection.data);
}

void ColorShader::LoadModelViewMatrix(const math::Matrix& modelView)
{
    glUniformMatrix4fv(mMVMatrixLocation, 1, GL_FALSE, modelView.data);
}

void ColorShader::SetPointSize(float size)
{
    glUniform1f(m_pointSizeLocation, size);
}

unsigned int ColorShader::GetPositionAttributeLocation() const
{
    return mPositionAttributeLocation;
}

unsigned int ColorShader::GetColorAttributeLocation() const
{
    return mColorAttributeLocation;
}

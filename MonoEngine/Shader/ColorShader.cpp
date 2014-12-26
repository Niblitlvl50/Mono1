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

    "attribute vec2 vertexPosition;"
    "attribute vec4 vertexColor;"

    "uniform mat4 mv_matrix;"
    "uniform mat4 p_matrix;"

    "varying vec4 color;"

    "void main()"
    "{"
    "    gl_Position = p_matrix * mv_matrix * vec4(vertexPosition, 0.0, 1.0);"
    "    color = vertexColor;"
    "}";

    constexpr const char* fragmentSource =

    "varying vec4 color;"

    "void main()"
    "{"
    "    gl_FragColor = color;"
    "}";
}


using namespace mono;

ColorShader::ColorShader()
{
    const GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
    const GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

    mProgram = LinkProgram(vertexShader, fragmentShader);

    mMVMatrixLocation = glGetUniformLocation(mProgram, "mv_matrix");
    mPMatrixLocation = glGetUniformLocation(mProgram, "p_matrix");

    mPositionAttributeLocation = glGetAttribLocation(mProgram, "vertexPosition");
    mColorAttributeLocation = glGetAttribLocation(mProgram, "vertexColor");
}

void ColorShader::Use()
{
    glUseProgram(mProgram);
}

void ColorShader::Clear()
{
    glUseProgram(0);
}

unsigned int ColorShader::GetShaderId() const
{
    return mProgram;
}

void ColorShader::LoadMatrices(const math::Matrix& projection, const math::Matrix& modelview)
{
    glUniformMatrix4fv(mPMatrixLocation, 1, GL_FALSE, projection.data);
    glUniformMatrix4fv(mMVMatrixLocation, 1, GL_FALSE, modelview.data);
}

int ColorShader::GetPositionAttributeLocation() const
{
    return mPositionAttributeLocation;
}

int ColorShader::GetColorAttributeLocation() const
{
    return mColorAttributeLocation;
}

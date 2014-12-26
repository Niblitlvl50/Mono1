//
//  ColorShader.cpp
//  MonoOSX
//
//  Created by Niklas Damberg on 25/12/14.
//
//

#include "ColorShader.h"
#include "IShader.h"
#include "ShaderFactory.h"
#include "SysOpenGL.h"
#include "Matrix.h"


namespace
{
    constexpr const char* vertexSource =
    ""
    "attribute vec2 vertexPosition;"
    "attribute vec4 vertexColor;"
    ""
    "uniform mat4 mv_matrix;"
    "uniform mat4 p_matrix;"
    ""
    "varying vec4 color;"
    ""
    "void main()"
    "{"
    "    gl_Position = p_matrix * mv_matrix * vec4(vertexPosition, 0.0, 1.0);"
    "    color = vertexColor;"
    "}";

    constexpr const char* fragmentSource =
    ""
    "varying vec4 color;"
    ""
    "void main()"
    "{"
    "    gl_FragColor = color;"
    "}";
}


using namespace mono;

ColorShader::ColorShader()
{
    mShader = mono::CreateShader(vertexSource, fragmentSource);
    mMVMatrixLocation = mShader->GetUniformLocation("mv_matrix");
    mPMatrixLocation = mShader->GetUniformLocation("p_matrix");
    mPositionAttributeLocation = mShader->GetAttribLocation("vertexPosition");
    mColorAttributeLocation = mShader->GetAttribLocation("vertexColor");
}

int ColorShader::GetMVMatrixLocation() const
{
    return mMVMatrixLocation;
}

int ColorShader::GetPMatrixLocation() const
{
    return mPMatrixLocation;
}

int ColorShader::GetPositionAttributeLocation() const
{
    return mPositionAttributeLocation;
}

int ColorShader::GetColorAttributeLocation() const
{
    return mColorAttributeLocation;
}

void ColorShader::LoadMatrices(const math::Matrix& projection, const math::Matrix& modelview)
{
    glUniformMatrix4fv(mPMatrixLocation, 1, GL_FALSE, projection.data);
    glUniformMatrix4fv(mMVMatrixLocation, 1, GL_FALSE, modelview.data);
}

void ColorShader::Use()
{
    mShader->Use();
}

void ColorShader::Clear()
{
    mShader->Clear();
}

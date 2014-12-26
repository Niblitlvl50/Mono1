//
//  TextureShader.cpp
//  MonoOSX
//
//  Created by Niklas Damberg on 26/12/14.
//
//

#include "TextureShader.h"
#include "IShader.h"
#include "ShaderFactory.h"
#include "SysOpenGL.h"
#include "Matrix.h"
#include "Color.h"


namespace
{
    constexpr const char* vertexSource =

    "attribute vec2 vertexPosition;"
    "attribute vec2 textureCoord;"

    "uniform mat4 mv_matrix;"
    "uniform mat4 p_matrix;"

    "varying vec2 vTextureCoord;"

    "void main()"
    "{"
    "    gl_Position = p_matrix * mv_matrix * vec4(vertexPosition, 0.0, 1.0);"
    "    vTextureCoord = textureCoord;"
    "}";

    constexpr const char* fragmentSource =

    "varying vec2 vTextureCoord;"

    "uniform vec4 colorShade;"
    "uniform sampler2D sampler;"
    "uniform bool isAlphaTexture;"

    "void main()"
    "{"
    "    if(isAlphaTexture)"
    "        gl_FragColor = texture2D(sampler, vTextureCoord).aaaa * colorShade;"
    "    else"
    "        gl_FragColor = texture2D(sampler, vTextureCoord) * colorShade;"
    "}";
}


using namespace mono;

TextureShader::TextureShader()
{
    mShader = mono::CreateShader(vertexSource, fragmentSource);

    mMVMatrixLocation = mShader->GetUniformLocation("mv_matrix");
    mPMatrixLocation = mShader->GetUniformLocation("p_matrix");
    mSamplerLocation = mShader->GetUniformLocation("sampler");
    mColorShadeLocation = mShader->GetUniformLocation("colorShade");
    mIsAlphaTextureLocation = mShader->GetUniformLocation("isAlphaTexture");

    mPositionAttributeLocation = mShader->GetAttribLocation("vertexPosition");
    mTextureAttributeLocation = mShader->GetAttribLocation("textureCoord");
}

int TextureShader::GetMVMatrixLocation() const
{
    return mMVMatrixLocation;
}

int TextureShader::GetPMatrixLocation() const
{
    return mPMatrixLocation;
}

int TextureShader::GetPositionAttributeLocation() const
{
    return mPositionAttributeLocation;
}

int TextureShader::GetTextureAttributeLocation() const
{
    return mTextureAttributeLocation;
}

void TextureShader::LoadMatrices(const math::Matrix& projection, const math::Matrix& modelview)
{
    glUniformMatrix4fv(mPMatrixLocation, 1, GL_FALSE, projection.data);
    glUniformMatrix4fv(mMVMatrixLocation, 1, GL_FALSE, modelview.data);
}

void TextureShader::SetShade(const mono::Color& shade)
{
    glUniform4f(mColorShadeLocation, shade.red, shade.green, shade.blue, shade.alpha);
}

void TextureShader::SetAlphaTexture(bool isAlpha)
{
    glUniform1i(mIsAlphaTextureLocation, isAlpha);
}

void TextureShader::Use()
{
    mShader->Use();
}

void TextureShader::Clear()
{
    mShader->Clear();
}

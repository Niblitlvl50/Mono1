
#include "PointSpriteShader.h"
#include "ShaderFunctions.h"
#include "System/OpenGL.h"
#include "Math/Matrix.h"


namespace
{
    constexpr const char* vertexSource =

#ifdef __IPHONEOS__
    "precision mediump float;"
#endif

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

#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    "#version 120\n"
#endif

#ifdef __IPHONEOS__
    "precision mediump float;"
#endif

    "varying vec4 color;"
    "uniform sampler2D sampler;"

    "void main()"
    "{"
    "    gl_FragColor = texture2D(sampler, gl_PointCoord) * color;"
    "}";
}


using namespace mono;

PointSpriteShader::PointSpriteShader()
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

void PointSpriteShader::Use()
{
    glUseProgram(mProgram);
}

unsigned int PointSpriteShader::GetShaderId() const
{
    return mProgram;
}

void PointSpriteShader::LoadProjectionMatrix(const math::Matrix& projection)
{
    glUniformMatrix4fv(mPMatrixLocation, 1, GL_FALSE, projection.data);
}

void PointSpriteShader::LoadModelViewMatrix(const math::Matrix& modelView)
{
    glUniformMatrix4fv(mMVMatrixLocation, 1, GL_FALSE, modelView.data);
}

void PointSpriteShader::SetPointSize(float size)
{
    glUniform1f(m_pointSizeLocation, size);
}

unsigned int PointSpriteShader::GetPositionAttributeLocation() const
{
    return mPositionAttributeLocation;
}

unsigned int PointSpriteShader::GetColorAttributeLocation() const
{
    return mColorAttributeLocation;
}

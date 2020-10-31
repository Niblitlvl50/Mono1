
#include "PointSpriteShader.h"
#include "Shader.h"

namespace
{
    constexpr const char* vertex_source = R"(
    #ifdef __IPHONEOS__
        precision mediump float;
    #endif

        uniform float total_time;
        uniform float delta_time;
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        attribute vec2 vertex_position;
        attribute float vertex_rotation;
        attribute vec4 vertex_color;
        attribute float point_size;

        varying vec4 color;
        varying float rotation;

        void main()
        {
            gl_Position = projection * view * model * vec4(vertex_position, 0.0, 1.0);
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

std::unique_ptr<IShader> PointSpriteShader::MakeShader()
{
    return std::make_unique<Shader>(vertex_source, fragment_source);
}

uint32_t PointSpriteShader::GetPositionAttribute(IShader* shader)
{
    return shader->GetAttributeLocation("vertex_position");
}

uint32_t PointSpriteShader::GetRotationAttribute(IShader* shader)
{
    return shader->GetAttributeLocation("vertex_rotation");
}

uint32_t PointSpriteShader::GetColorAttribute(IShader* shader)
{
    return shader->GetAttributeLocation("vertex_color");
}

uint32_t PointSpriteShader::GetPointSizeAttribute(IShader* shader)
{
    return shader->GetAttributeLocation("point_size");
}

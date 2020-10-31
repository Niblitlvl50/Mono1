
#include "ColorShader.h"
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

        uniform float point_size;

        attribute vec2 vertex_position;
        attribute vec4 vertex_color;

        varying vec4 color;

        void main()
        {
            gl_Position = projection * view * model * vec4(vertex_position, 0.0, 1.0);
            gl_PointSize = point_size;
            color = vertex_color;
        }
    )";

    constexpr const char* fragment_source = R"(
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

std::unique_ptr<IShader> ColorShader::MakeShader()
{
    return std::make_unique<Shader>(vertex_source, fragment_source);
}

void ColorShader::SetPointSize(IShader* shader, float size)
{
    shader->SetValue("point_size", size);
}

uint32_t ColorShader::GetPositionAttribute(IShader* shader)
{
    return shader->GetAttributeLocation("vertex_position");
}

uint32_t ColorShader::GetColorAttribute(IShader* shader)
{
    return shader->GetAttributeLocation("vertex_color");
}

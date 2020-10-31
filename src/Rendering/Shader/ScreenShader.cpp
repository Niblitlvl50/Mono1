
#include "ScreenShader.h"
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
        attribute vec2 texture_coord;

        varying vec2 v_texture_coord;

        void main()
        {
            gl_Position = vec4(vertex_position, 0.0, 1.0);
            v_texture_coord = texture_coord;
        }
    )";

    constexpr const char* fragment_source = R"(
    #ifdef __IPHONEOS__
        precision mediump float;
    #endif

        varying vec2 v_texture_coord;
        uniform sampler2D sampler;

        void main()
        {
            gl_FragColor = vec4(texture2D(sampler, v_texture_coord).rgb, 1.0);
            //gl_FragColor = vec4(vec3(1.0 - texture2D(sampler, v_texture_coord)), 1.0);
        }
    )";
}

using namespace mono;

std::unique_ptr<mono::IShader> ScreenShader::MakeShader()
{
    return std::make_unique<Shader>(vertex_source, fragment_source);
}

uint32_t ScreenShader::GetPositionAttribute(IShader* shader)
{
    return shader->GetAttributeLocation("vertex_position");
}

uint32_t ScreenShader::GetTextureAttribute(IShader* shader)
{
    return shader->GetAttributeLocation("texture_coord");
}

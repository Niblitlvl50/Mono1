
#include "ScreenShader.h"
#include "Shader.h"

namespace
{
    constexpr const char* vertex_source = R"(
        uniform float total_time;
        uniform float delta_time;
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        attribute vec2 vertex_position;
        attribute vec2 texture_coord;

        varying vec2 v_vertex_coord;
        varying vec2 v_texture_coord;

        void main()
        {
            gl_Position = vec4(vertex_position, 0.0, 1.0);
            v_texture_coord = texture_coord;
            v_vertex_coord = vertex_position;
        }
    )";

    constexpr const char* fragment_source = R"(
        varying vec2 v_texture_coord;
        varying vec2 v_vertex_coord;

        uniform bool fade_corners;
        uniform sampler2D sampler;

        void main()
        {
            vec4 frag_color = vec4(texture2D(sampler, v_texture_coord).rgb, 1.0);

            if(fade_corners)
            {
                float unit_length = length(vec2(1.0, 1.0));
                float length = length(v_vertex_coord);
                float length_diff = unit_length - length;

                float alpha = 1.0;
                if(length > 1.0)
                {
                    float range = unit_length - 1.0;
                    alpha = length_diff / range;
                    frag_color = mix(vec4(0.0, 0.0, 0.0, 1.0), frag_color, vec4(alpha));
                }
            }

            gl_FragColor = frag_color;

            // Invert colors
            //gl_FragColor = vec4(vec3(1.0 - texture2D(sampler, v_texture_coord)), 1.0);
        }
    )";
}

using namespace mono;

std::unique_ptr<mono::IShader> ScreenShader::MakeShader()
{
    return std::make_unique<Shader>(vertex_source, fragment_source);
}

void ScreenShader::FadeCorners(IShader* shader, bool enable)
{
    shader->SetValue("fade_corners", enable);
}

uint32_t ScreenShader::GetPositionAttribute(IShader* shader)
{
    return shader->GetAttributeLocation("vertex_position");
}

uint32_t ScreenShader::GetTextureAttribute(IShader* shader)
{
    return shader->GetAttributeLocation("texture_coord");
}

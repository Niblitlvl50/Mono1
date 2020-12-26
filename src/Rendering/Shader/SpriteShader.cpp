
#include "SpriteShader.h"
#include "Shader.h"
#include "Rendering/Color.h"
#include "Math/Vector.h"

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

        uniform bool wind_sway_enabled;

        attribute vec2 vertex_position;
        attribute vec2 texture_coord;
        attribute float vertex_height;

        varying vec2 v_texture_coord;

        void main()
        {
            vec2 wind_sway_offset = vec2(0.0, 0.0);
            if(wind_sway_enabled)
            {
                // Use world x as noise parameter to make some difference.
                float noise = abs(noise1(model[3][0]));
                wind_sway_offset.x = sin(total_time * noise * 3.0) * (vertex_height * 0.025);
            }

            gl_Position = projection * view * model * vec4(vertex_position + wind_sway_offset, 0.0, 1.0);
            v_texture_coord = texture_coord;
        }
    )";

    constexpr const char* fragment_source = R"(
    #ifdef __IPHONEOS__
        precision mediump float;
    #endif

        varying vec2 v_texture_coord;

        uniform vec4 color_shade;
        uniform bool flash_sprite;

        uniform sampler2D sampler;

        void main()
        {
            vec4 frag_color = texture2D(sampler, v_texture_coord) * color_shade;
            if(flash_sprite)
                frag_color.rgb = vec3(1.0);

            gl_FragColor = frag_color;
        }
    )";
}

using namespace mono;

std::unique_ptr<mono::IShader> SpriteShader::MakeShader()
{
    return std::make_unique<Shader>(vertex_source, fragment_source);
}

void SpriteShader::SetShade(IShader* shader, const mono::Color::RGBA& color)
{
    shader->SetValue("color_shade", color);
}

void SpriteShader::SetWindSway(IShader* shader, bool enable_wind)
{
    shader->SetValue("wind_sway_enabled", enable_wind);
}

void SpriteShader::SetFlashSprite(IShader* shader, bool flash_sprite)
{
    shader->SetValue("flash_sprite", flash_sprite);
}

uint32_t SpriteShader::GetPositionAttribute(IShader* shader)
{
    return shader->GetAttributeLocation("vertex_position");
}

uint32_t SpriteShader::GetTextureAttribute(IShader* shader)
{
    return shader->GetAttributeLocation("texture_coord");
}

uint32_t SpriteShader::GetHeightAttribute(IShader* shader)
{
    return shader->GetAttributeLocation("vertex_height");
}

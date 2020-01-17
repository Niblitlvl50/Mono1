
#include "TextureShader.h"
#include "Shader.h"
#include "Rendering/Color.h"
#include "System/open_gl.h"

namespace
{
    constexpr const char* vertex_source = R"(
    #ifdef __IPHONEOS__
        precision mediump float;
    #endif

        attribute vec2 vertex_position;
        attribute vec2 texture_coord;

        uniform mat4 mv_matrix;
        uniform mat4 p_matrix;

        varying vec2 v_texture_coord;

        void main()
        {
            gl_Position = p_matrix * mv_matrix * vec4(vertex_position, 0.0, 1.0);
            v_texture_coord = texture_coord;
        }
    )";

    constexpr const char* fragment_source = R"(
    #ifdef __IPHONEOS__
        precision mediump float;
    #endif

        varying vec2 v_texture_coord;

        uniform vec4 color_shade;
        uniform sampler2D sampler;
        uniform bool is_alpha_texture;
        uniform float texture_offset;

        void main()
        {
            if(is_alpha_texture)
                gl_FragColor = texture2D(sampler, v_texture_coord).aaaa * color_shade;
            else
                gl_FragColor = texture2D(sampler, v_texture_coord) * color_shade;
        }
    )";
}

using namespace mono;

std::unique_ptr<mono::IShader> TextureShader::MakeShader()
{
    return std::make_unique<Shader>(vertex_source, fragment_source);
}

void TextureShader::SetShade(IShader* shader, const mono::Color::RGBA& color)
{
    shader->SetValue("color_shade", color);
}

void TextureShader::SetAlphaTexture(IShader* shader, bool is_alpha)
{
    shader->SetValue("is_alpha_texture", is_alpha);
}

void TextureShader::SetTextureOffset(IShader* shader, float offset)
{
    shader->SetValue("texture_offset", offset);
}

uint32_t TextureShader::GetPositionAttribute(IShader* shader)
{
    return shader->GetAttributeLocation("vertex_position");
}

uint32_t TextureShader::GetTextureAttribute(IShader* shader)
{
    return shader->GetAttributeLocation("texture_coord");
}

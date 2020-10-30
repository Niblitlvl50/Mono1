
#include "ImGuiShader.h"
#include "Shader.h"

using namespace mono;

namespace
{
    constexpr const char* vertex_source = R"(
    #ifdef __IPHONEOS__
        precision mediump float;
    #endif

        uniform float total_time;
        uniform float delta_time;
        uniform mat4 mv_matrix;
        uniform mat4 p_matrix;

        attribute vec2 Position;
        attribute vec2 UV;
        attribute vec4 Color;

        varying vec2 Frag_UV;
        varying vec4 Frag_Color;

        void main()
        {
            Frag_UV = UV;
            Frag_Color = Color;
            gl_Position = p_matrix * mv_matrix * vec4(Position.xy, 0.0, 1.0);
        }
    )";

    constexpr const char* fragment_source = R"(
    #ifdef __IPHONEOS__
        precision mediump float;
    #endif

        uniform sampler2D Texture;

        varying vec2 Frag_UV;
        varying vec4 Frag_Color;

        void main()
        {
           gl_FragColor = Frag_Color * texture2D(Texture, Frag_UV.st);
        }
    )";
}

std::unique_ptr<IShader> ImGuiShader::MakeShader()
{
    return std::make_unique<Shader>(vertex_source, fragment_source);
}

void ImGuiShader::SetTextureValue(IShader* shader, int value)
{
    shader->SetValue("Texture", value);
}

uint32_t ImGuiShader::GetPositionAttribute(IShader* shader)
{
    return shader->GetAttributeLocation("Position");
}

uint32_t ImGuiShader::GetColorAttribute(IShader* shader)
{
    return shader->GetAttributeLocation("Color");
}

uint32_t ImGuiShader::GetTextureAttribute(IShader* shader)
{
    return shader->GetAttributeLocation("UV");
}

uint32_t ImGuiShader::GetTextureLocation(IShader* shader)
{
    return shader->GetAttributeLocation("Texture");
}

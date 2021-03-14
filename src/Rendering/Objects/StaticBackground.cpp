
#include "StaticBackground.h"

#include "Math/MathFunctions.h"
#include "Math/Quad.h"
#include "Rendering/RenderSystem.h"
#include "Rendering/RenderBuffer/BufferFactory.h"
#include "Rendering/IRenderer.h"
#include "Rendering/Texture/ITexture.h"
#include "Rendering/Texture/ITextureFactory.h"

using namespace mono;

StaticBackground::StaticBackground()
{}

StaticBackground::StaticBackground(const char* background_texture)
{
    Load(background_texture);
}

void StaticBackground::Draw(mono::IRenderer& renderer) const
{
    if(m_texture)
        renderer.DrawGeometry(m_vertex_buffer.get(), m_texture_buffer.get(), m_index_buffer.get(), m_texture.get(), 6);
}

math::Quad StaticBackground::BoundingBox() const
{
    return math::InfQuad;
}

void StaticBackground::Load(const char* background_texture)
{
    m_texture = mono::GetTextureFactory()->CreateTexture(background_texture);

    constexpr math::Quad background_bb = math::Quad(-100.0f, -100.0f, 100.0f, 100.0f);

    const math::Vector vertices[] = {
        math::BottomLeft(background_bb),
        math::TopLeft(background_bb),
        math::TopRight(background_bb),
        math::BottomRight(background_bb)
    };

    const uint16_t indices[] = {
        0, 1, 2,
        0, 2, 3,
    };

    math::Vector uv_coordinates[] = {
        math::ZeroVec,
        math::ZeroVec,
        math::ZeroVec,
        math::ZeroVec,
    };

    const float pixels_per_meter = mono::PixelsPerMeter();
    const float background_width = math::Width(background_bb);
    const float background_height = math::Height(background_bb);
    const math::Vector repeate =
        math::Vector(background_width / m_texture->Width(), background_height / m_texture->Height()) * pixels_per_meter;

    for(size_t index = 0; index < std::size(vertices); ++index)
        uv_coordinates[index] = math::MapVectorInQuad(vertices[index], background_bb) * repeate;

    m_vertex_buffer = mono::CreateRenderBuffer(mono::BufferType::STATIC, mono::BufferData::FLOAT, 2, std::size(vertices), vertices);
    m_texture_buffer = mono::CreateRenderBuffer(mono::BufferType::STATIC, mono::BufferData::FLOAT, 2, std::size(uv_coordinates), uv_coordinates);
    m_index_buffer = mono::CreateElementBuffer(mono::BufferType::STATIC, std::size(indices), indices);
}

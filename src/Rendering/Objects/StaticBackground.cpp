
#include "StaticBackground.h"

#include "Math/MathFunctions.h"
#include "Math/Quad.h"
#include "Rendering/RenderSystem.h"
#include "Rendering/RenderBuffer/BufferFactory.h"
#include "Rendering/IRenderer.h"
#include "Rendering/Texture/ITexture.h"
#include "Rendering/Texture/ITextureFactory.h"
#include "Rendering/Color.h"

using namespace mono;

StaticBackground::StaticBackground()
    : m_mode(TextureModeFlags(0))
{ }

StaticBackground::StaticBackground(const char* texture_filename, uint32_t texture_mode_flags)
{
    Load(texture_filename, texture_mode_flags);
}

StaticBackground::StaticBackground(const math::Vector& size, const char* texture_filename, uint32_t texture_mode_flags)
{
    Load(size, texture_filename, texture_mode_flags);
}

StaticBackground::StaticBackground(const std::vector<math::Vector>& vertices, const char* texture_filename, uint32_t texture_mode_flags)
{
    Load(vertices, texture_filename, texture_mode_flags);
}

void StaticBackground::Load(const char* texture_filename, uint32_t texture_mode_flags)
{
    constexpr math::Quad background_bb = math::Quad(-100.0f, -100.0f, 100.0f, 100.0f);

    const std::vector<math::Vector> vertices = {
        math::BottomLeft(background_bb),
        math::TopLeft(background_bb),
        math::TopRight(background_bb),
        math::BottomRight(background_bb)
    };

    Load(vertices, texture_filename, texture_mode_flags);
}

void StaticBackground::Load(const math::Vector& size, const char* texture_filename, uint32_t texture_mode_flags)
{
    const math::Vector half_size = size / 2.0f;

    const std::vector<math::Vector> vertices = {
        -half_size,
        { -half_size.x, half_size.y },
        half_size,
        { half_size.x, -half_size.y }
    };

    Load(vertices, texture_filename, texture_mode_flags);
}

void StaticBackground::Load(const std::vector<math::Vector>& vertices, const char* texture_filename, uint32_t texture_mode_flags)
{
    m_mode = TextureModeFlags(texture_mode_flags);
    m_texture = mono::GetTextureFactory()->CreateTexture(texture_filename);

    math::Quad bounding_box = math::Quad(math::INF, math::INF, -math::INF, -math::INF);
    for(const math::Vector& vertex : vertices)
        bounding_box |= vertex;

    math::Vector repeate = math::Vector(1.0f, 1.0f);
    
    if(texture_mode_flags & TextureModeFlags::REPEAT)
    {
        const float pixels_per_meter = mono::PixelsPerMeter();
        const float background_width = math::Width(bounding_box);
        const float background_height = math::Height(bounding_box);
        repeate = math::Vector(
            background_width / m_texture->Width(),
            background_height / m_texture->Height()) * pixels_per_meter;
    }

    std::vector<math::Vector> uv_coordinates;
    uv_coordinates.reserve(vertices.size());

    for(const math::Vector& vertex : vertices)
        uv_coordinates.push_back(math::MapVectorInQuad(vertex, bounding_box) * repeate);

    const uint16_t indices[] = {
        0, 1, 2,
        0, 2, 3,
    };

    m_vertex_buffer = mono::CreateRenderBuffer(mono::BufferType::STATIC, mono::BufferData::FLOAT, 2, vertices.size(), vertices.data());
    m_texture_buffer = mono::CreateRenderBuffer(mono::BufferType::STATIC, mono::BufferData::FLOAT, 2, uv_coordinates.size(), uv_coordinates.data());
    m_index_buffer = mono::CreateElementBuffer(mono::BufferType::STATIC, std::size(indices), indices);
}

void StaticBackground::Clear()
{
    m_vertex_buffer = nullptr;
    m_texture_buffer = nullptr;
    m_index_buffer = nullptr;
    m_texture = nullptr;
}

void StaticBackground::Draw(mono::IRenderer& renderer) const
{
    if(m_texture)
    {
        const bool blur = (m_mode & TextureModeFlags::BLUR);
        renderer.DrawGeometry(
            m_vertex_buffer.get(),
            m_texture_buffer.get(),
            m_index_buffer.get(),
            m_texture.get(),
            mono::Color::WHITE,
            blur,
            6);
    }
}

math::Quad StaticBackground::BoundingBox() const
{
    return math::InfQuad;
}

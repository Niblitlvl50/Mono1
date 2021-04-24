
#pragma once

#include "Rendering/IDrawable.h"
#include "Rendering/RenderBuffer/IRenderBuffer.h"
#include "Rendering/Texture/ITextureFactory.h"

#include <vector>
#include <string>

struct TerrainDrawData
{
    mono::ITexturePtr texture;
    size_t offset;
    size_t count;
};

struct PolygonData
{
    std::string texture_name;
    std::vector<math::Vector> vertices;
};

class StaticTerrainBlock : public mono::IDrawable
{
public:

    StaticTerrainBlock(size_t vertex_count, size_t polygon_count);

    void AddPolygon(const PolygonData& polygon);
    void Draw(mono::IRenderer& renderer) const override;
    math::Quad BoundingBox() const override;

private:

    unsigned int m_index;

    std::unique_ptr<mono::IRenderBuffer> m_vertex_buffer;
    std::unique_ptr<mono::IRenderBuffer> m_texture_buffer;
    std::unique_ptr<mono::IRenderBuffer> m_color_buffer;

    std::unique_ptr<mono::IRenderBuffer> m_vertex_buffer_2;
    std::unique_ptr<mono::IRenderBuffer> m_color_buffer_2;
    std::unique_ptr<mono::IElementBuffer> m_index_buffer;

    std::vector<TerrainDrawData> m_draw_data;

    size_t m_vertices;
};

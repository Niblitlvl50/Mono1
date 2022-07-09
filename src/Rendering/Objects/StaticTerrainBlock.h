
#pragma once

#include "Rendering/IDrawable.h"
#include "Rendering/RenderBuffer/IRenderBuffer.h"
#include "Rendering/Texture/ITextureFactory.h"

#include <vector>
#include <string>

struct TerrainDrawData
{
    mono::ITexturePtr texture;
    uint32_t offset;
    uint32_t count;
};

class StaticTerrainBlock : public mono::IDrawable
{
public:

    StaticTerrainBlock(uint32_t vertex_count, uint32_t polygon_count);

    void AddPolygon(const std::vector<math::Vector>& vertices, const std::string& texture_name);
    void Draw(mono::IRenderer& renderer) const override;
    math::Quad BoundingBox() const override;

private:

    uint32_t m_index;

    std::unique_ptr<mono::IRenderBuffer> m_vertex_buffer;
    std::unique_ptr<mono::IRenderBuffer> m_texture_buffer;
    std::unique_ptr<mono::IRenderBuffer> m_color_buffer;

    std::unique_ptr<mono::IRenderBuffer> m_vertex_buffer_2;
    std::unique_ptr<mono::IRenderBuffer> m_color_buffer_2;
    std::unique_ptr<mono::IElementBuffer> m_index_buffer;

    std::vector<TerrainDrawData> m_draw_data;

    uint32_t m_vertices;
};

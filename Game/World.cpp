
#include "World.h"
#include "CollisionConfiguration.h"

#include "Physics/IBody.h"
#include "Physics/IShape.h"
#include "Physics/CMFactory.h"
#include "Physics/CMPhysicsData.h"

#include "Zone/IPhysicsZone.h"
#include "RenderLayers.h"

#include "Rendering/IDrawable.h"
#include "Rendering/IRenderer.h"
#include "Rendering/IRenderBuffer.h"
#include "Rendering/BufferFactory.h"
#include "Rendering/Texture/TextureFactory.h"

#include "Math/Quad.h"
#include "Math/MathFunctions.h"

namespace
{
    struct TerrainDrawData
    {
        mono::ITexturePtr texture;
        size_t offset;
        size_t count;
    };

    class StaticTerrainBlock : public mono::IDrawable
    {
    public:

        StaticTerrainBlock(size_t vertex_count, size_t polygon_count)
            : m_index(0)
        {
            m_static_physics.body = mono::PhysicsFactory::CreateStaticBody();

            m_draw_data.reserve(polygon_count);
            m_vertex_buffer = mono::CreateRenderBuffer(mono::BufferType::STATIC, mono::BufferData::FLOAT, vertex_count * 2);
            m_texture_buffer = mono::CreateRenderBuffer(mono::BufferType::STATIC, mono::BufferData::FLOAT, vertex_count * 2);
        }

        void AddPolygon(const world::PolygonData& polygon)
        {
            mono::IShapePtr shape = mono::PhysicsFactory::CreateShape(m_static_physics.body, polygon.vertices, math::zeroVec);
            shape->SetCollisionFilter(game::CollisionCategory::STATIC, game::STATIC_MASK);
            m_static_physics.shapes.push_back(shape);

            TerrainDrawData draw_data;
            draw_data.offset = m_index;
            draw_data.count = polygon.vertices.size();
            draw_data.texture = mono::CreateTexture(polygon.texture);

            m_draw_data.emplace_back(draw_data);

            std::vector<math::Vector> texture_coordinates;
            texture_coordinates.reserve(polygon.vertices.size());

            math::Quad bounding_box = math::Quad(math::INF, math::INF, -math::INF, -math::INF);
            for(const math::Vector& vertex : polygon.vertices)
                bounding_box |= vertex;

            for(const math::Vector& vertex : polygon.vertices)
                texture_coordinates.push_back(math::MapVectorInQuad(vertex, bounding_box) * polygon.texture_repeate);

            m_vertex_buffer->UpdateData(polygon.vertices.data(), draw_data.offset * 2, draw_data.count * 2);
            m_texture_buffer->UpdateData(texture_coordinates.data(), draw_data.offset * 2, draw_data.count * 2);

            m_index += draw_data.count;
        }

        virtual void doDraw(mono::IRenderer& renderer) const
        {
            for(const TerrainDrawData& draw_data : m_draw_data)
            {
                renderer.DrawGeometry(m_vertex_buffer.get(),
                                      m_texture_buffer.get(),
                                      draw_data.offset,
                                      draw_data.count,
                                      draw_data.texture);
            }
        }

        virtual math::Quad BoundingBox() const
        {
            return math::Quad(-math::INF, -math::INF, math::INF, math::INF);
        }

        unsigned int m_index;
        mono::PhysicsData m_static_physics;

        std::unique_ptr<mono::IRenderBuffer> m_vertex_buffer;
        std::unique_ptr<mono::IRenderBuffer> m_texture_buffer;
        std::vector<TerrainDrawData> m_draw_data;
    };
}

void game::LoadWorld(mono::IPhysicsZone* zone, const std::vector<world::PolygonData>& polygons)
{
    size_t count = 0;

    for(const world::PolygonData& polygon : polygons)
        count += polygon.vertices.size();

    auto static_terrain = std::make_shared<StaticTerrainBlock>(count, polygons.size());

    for(const world::PolygonData& polygon : polygons)
        static_terrain->AddPolygon(polygon);

    zone->AddDrawable(static_terrain, BACKGROUND);
    zone->AddPhysicsData(static_terrain->m_static_physics);
}

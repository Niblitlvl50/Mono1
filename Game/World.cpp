
#include "World.h"
#include "CollisionConfiguration.h"

#include "Physics/IBody.h"
#include "Physics/IShape.h"
#include "Physics/CMFactory.h"
#include "Physics/CMObject.h"

#include "IDrawable.h"
#include "IRenderer.h"
#include "IPhysicsZone.h"
#include "RenderLayers.h"
#include "Texture/TextureFactory.h"

#include "Math/Quad.h"
#include "Math/MathFunctions.h"

namespace
{
    struct TerrainDrawData
    {
        mono::ITexturePtr texture;
        size_t index;
        size_t count;
    };

    class StaticTerrainBlock : public mono::IDrawable
    {
    public:

        StaticTerrainBlock(size_t vertex_count, size_t polygon_count)
        {
            m_static_physics.body = mono::PhysicsFactory::CreateStaticBody();

            m_vertices.reserve(vertex_count);
            m_texture_coordinates.reserve(vertex_count);
            m_draw_data.reserve(polygon_count);
        }

        void AddPolygon(const world::PolygonData& polygon)
        {
            mono::IShapePtr shape = mono::PhysicsFactory::CreateShape(m_static_physics.body, polygon.vertices, math::zeroVec);
            shape->SetCollisionFilter(game::CollisionCategory::STATIC, game::STATIC_MASK);
            m_static_physics.shapes.push_back(shape);

            TerrainDrawData draw_data;
            draw_data.texture = mono::CreateTexture(polygon.texture);
            draw_data.index = m_vertices.size();
            draw_data.count = polygon.vertices.size();

            m_draw_data.emplace_back(draw_data);

            m_vertices.insert(m_vertices.end(), polygon.vertices.begin(), polygon.vertices.end());

            math::Quad bounding_box = math::Quad(math::INF, math::INF, -math::INF, -math::INF);
            for(const math::Vector& vertex : polygon.vertices)
                bounding_box |= vertex;

            for(const math::Vector& vertex : polygon.vertices)
                m_texture_coordinates.push_back(math::MapVectorInQuad(vertex, bounding_box) * polygon.texture_repeate);
        }

        virtual void doDraw(mono::IRenderer& renderer) const
        {
            for(const TerrainDrawData& draw_data : m_draw_data)
            {
                std::vector<unsigned short> indices;
                indices.reserve(draw_data.count);

                for(unsigned int index = 0; index < draw_data.count; ++index)
                    indices.push_back(index + draw_data.index);

                renderer.DrawGeometry(m_vertices, m_texture_coordinates, indices, draw_data.texture);
            }
        }

        virtual math::Quad BoundingBox() const
        {
            return math::Quad(-math::INF, -math::INF, math::INF, math::INF);
        }

        mono::PhysicsData m_static_physics;

        std::vector<math::Vector> m_vertices;
        std::vector<math::Vector> m_texture_coordinates;
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
}

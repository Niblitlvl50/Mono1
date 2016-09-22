//
//  World.cpp
//  MonoiOS
//
//  Created by Niklas Damberg on 26/07/16.
//
//

#include "World.h"
#include "PhysicsEntityBase.h"

#include "Physics/CMIBody.h"
#include "Physics/CMIShape.h"
#include "Physics/CMFactory.h"

#include "IRenderer.h"
#include "IPhysicsZone.h"
#include "RenderLayers.h"
#include "Texture/TextureFactory.h"

#include "Color.h"
#include "Math/Quad.h"
#include "Math/MathFunctions.h"

namespace
{
    class TerrainBlock : public mono::PhysicsEntityBase
    {
    public:

        TerrainBlock(const world::PolygonData& polygon)
            : m_vertices(polygon.vertices)
        {
            m_texture = mono::CreateTexture(polygon.texture);

            mPhysicsObject.body = mono::PhysicsFactory::CreateStaticBody();
            mPhysicsObject.shapes.push_back(mono::PhysicsFactory::CreateShape(mPhysicsObject.body, polygon.vertices, math::zeroVec));

            m_boundingBox = math::Quad(math::INF, math::INF, -math::INF, -math::INF);

            for(const math::Vector2f& vertex : polygon.vertices)
                m_boundingBox |= vertex;

            for(const math::Vector2f& point : m_vertices)
                m_texture_coordinates.push_back(math::MapVectorInQuad(point, m_boundingBox) * polygon.texture_repeate);

            for(size_t index = 0; index < m_vertices.size(); ++index)
                m_indices.push_back(index);
        }

        virtual void Draw(mono::IRenderer& renderer) const
        {
            constexpr mono::Color::RGBA color(0.0f, 0.0f, 0.0f, 1.0f);
            renderer.DrawGeometry(m_vertices, m_texture_coordinates, m_indices, m_texture);
            renderer.DrawClosedPolyline(m_vertices, color, 4.0f);
        }

        virtual math::Quad BoundingBox() const
        {
            return m_boundingBox;
        }

        virtual void Update(unsigned int delta)
        { }

        const std::vector<math::Vector2f> m_vertices;
        std::vector<math::Vector2f> m_texture_coordinates;
        std::vector<unsigned short> m_indices;

        mono::ITexturePtr m_texture;
        math::Quad m_boundingBox;
    };
}

void game::LoadWorld(mono::IPhysicsZone* zone, const std::vector<world::PolygonData>& polygons)
{
    for(const world::PolygonData& polygon : polygons)
    {
        auto terrain = std::make_shared<TerrainBlock>(polygon);
        zone->AddPhysicsEntity(terrain, BACKGROUND);
    }
}

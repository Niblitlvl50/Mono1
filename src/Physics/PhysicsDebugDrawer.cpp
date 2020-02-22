
#include "PhysicsDebugDrawer.h"
#include "PhysicsSystem.h"
#include "CMSpace.h"
#include "Math/Quad.h"
#include "Rendering/Color.h"
#include "Rendering/IRenderer.h"

#include "chipmunk/chipmunk.h"

using namespace mono;

namespace
{
    struct CircleData
    {
        math::Vector position;
        float radius;
    };

    struct SegmentData
    {
        math::Vector start;
        math::Vector end;
    };

    struct PolygonData
    {
        std::vector<math::Vector> vertices;
    };

//    struct DotData
//    {
//        math::Vector position;
//    };

    struct DebugDrawCollection
    {
        std::vector<CircleData> circles;
        std::vector<math::Vector> segments;
        std::vector<math::Vector> fat_segments;
        std::vector<PolygonData> polygons;
        std::vector<math::Vector> dots;
    };

    cpSpaceDebugColor ConvertColor(const mono::Color::RGBA& input)
    {
        return { input.red, input.green, input.blue, input.alpha };
    }

    void DebugDrawCircle(cpVect pos, cpFloat angle, cpFloat radius, cpSpaceDebugColor outlineColor, cpSpaceDebugColor fillColor, cpDataPointer data)
    {
        CircleData circle_data;
        circle_data.position = math::Vector(pos.x, pos.y);
        circle_data.radius = radius;

        DebugDrawCollection* debug_collection = static_cast<DebugDrawCollection*>(data);
        debug_collection->circles.push_back(circle_data);
    }

    void DebugDrawSegment(cpVect a, cpVect b, cpSpaceDebugColor color, cpDataPointer data)
    {
        //SegmentData segment_data;
        //segment_data.start = math::Vector(a.x, a.y);
        //segment_data.end = math::Vector(b.x, b.y);

        DebugDrawCollection* debug_collection = static_cast<DebugDrawCollection*>(data);
        debug_collection->segments.push_back(math::Vector(a.x, a.y));
        debug_collection->segments.push_back(math::Vector(b.x, b.y));
    }

    void DebugDrawFatSegment(cpVect a, cpVect b, cpFloat radius, cpSpaceDebugColor outlineColor, cpSpaceDebugColor fillColor, cpDataPointer data)
    {
        //SegmentData segment_data;
        //segment_data.start = math::Vector(a.x, a.y);
        //segment_data.end = math::Vector(b.x, b.y);

        DebugDrawCollection* debug_collection = static_cast<DebugDrawCollection*>(data);
        debug_collection->fat_segments.push_back(math::Vector(a.x, a.y));
        debug_collection->fat_segments.push_back(math::Vector(b.x, b.y));
    }

    void DebugDrawPolygon(int count, const cpVect *verts, cpFloat radius, cpSpaceDebugColor outlineColor, cpSpaceDebugColor fillColor, cpDataPointer data)
    {
        PolygonData polygon_data;

        for(int index = 0; index < count; ++index)
        {
            const cpVect& vertex = verts[index];
            polygon_data.vertices.push_back(math::Vector(vertex.x, vertex.y));
        }

        DebugDrawCollection* debug_collection = static_cast<DebugDrawCollection*>(data);
        debug_collection->polygons.push_back(polygon_data);
    }

    void DebugDrawDot(cpFloat size, cpVect pos, cpSpaceDebugColor color, cpDataPointer data)
    {
        //DotData dot_data;
        //dot_data.position = math::Vector(pos.x, pos.y);

        DebugDrawCollection* debug_collection = static_cast<DebugDrawCollection*>(data);
        debug_collection->dots.push_back(math::Vector(pos.x, pos.y));
    }
    
    cpSpaceDebugColor DebugDrawColorForShape(cpShape *shape, cpDataPointer data)
    {
        return ConvertColor(mono::Color::RED);
    }
}

static_assert(uint32_t(CP_SPACE_DEBUG_DRAW_SHAPES) == mono::PhysicsDebugComponents::DRAW_SHAPES);
static_assert(uint32_t(CP_SPACE_DEBUG_DRAW_CONSTRAINTS) == mono::PhysicsDebugComponents::DRAW_CONSTRAINTS);
static_assert(uint32_t(CP_SPACE_DEBUG_DRAW_COLLISION_POINTS) == mono::PhysicsDebugComponents::DRAW_COLLISION_POINTS);

PhysicsDebugDrawer::PhysicsDebugDrawer(
    const bool& enabled, const uint32_t& debug_components, mono::PhysicsSystem* physics_system)
    : m_enabled(enabled)
    , m_debug_components(debug_components)
    , m_physics_system(physics_system)
{ }

void PhysicsDebugDrawer::doDraw(mono::IRenderer& renderer) const
{
    if(!m_enabled)
        return;

    DebugDrawCollection debug_collection;

    mono::PhysicsSpace* physics_space = m_physics_system->GetSpace();
    cpSpace* cm_space = physics_space->Handle();

    cpSpaceDebugDrawOptions options;
    options.flags = cpSpaceDebugDrawFlags(m_debug_components);
    options.shapeOutlineColor = ConvertColor(mono::Color::RED);
    options.constraintColor = ConvertColor(mono::Color::GREEN);
    options.collisionPointColor = ConvertColor(mono::Color::BLUE);
    options.drawCircle = DebugDrawCircle;
    options.drawSegment = DebugDrawSegment;
    options.drawFatSegment = DebugDrawFatSegment;
    options.drawPolygon = DebugDrawPolygon;
    options.drawDot = DebugDrawDot;
    options.colorForShape = DebugDrawColorForShape;
    options.data = &debug_collection;

    cpSpaceDebugDraw(cm_space, &options);

    if(!debug_collection.dots.empty())
        renderer.DrawPoints(debug_collection.dots, mono::Color::GREEN, 2.0f);

    for(const CircleData& circle_data : debug_collection.circles)
        renderer.DrawCircle(circle_data.position, circle_data.radius, 16, 1.0f, mono::Color::MAGENTA);

    if(!debug_collection.segments.empty())
        renderer.DrawLines(debug_collection.segments, mono::Color::MAGENTA, 1.0f);

    if(!debug_collection.fat_segments.empty())
        renderer.DrawLines(debug_collection.fat_segments, mono::Color::MAGENTA, 4.0f);

    for(const PolygonData& polygon_data : debug_collection.polygons)
        renderer.DrawClosedPolyline(polygon_data.vertices, mono::Color::MAGENTA, 2.0f);
}

math::Quad PhysicsDebugDrawer::BoundingBox() const
{
    return math::InfQuad;
}


#include "PhysicsDebugDrawer.h"
#include "PhysicsSystem.h"
#include "PhysicsSpace.h"
#include "IShape.h"
#include "Math/Quad.h"
#include "Math/Matrix.h"
#include "Rendering/Color.h"
#include "Rendering/IRenderer.h"

#include "System/System.h"
#include "EventHandler/EventHandler.h"
#include "Events/EventFuncFwd.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"

#include "chipmunk/chipmunk.h"
#include "chipmunk/chipmunk_structs.h"
#include "imgui/imgui.h"

#include <cstdio>

using namespace mono;

namespace
{
    struct CircleData
    {
        mono::Color::RGBA fill_color;
        mono::Color::RGBA outline_color;
        math::Vector position;
        float radius;
    };

    struct SegmentData
    {
        mono::Color::RGBA fill_color;
        mono::Color::RGBA outline_color;
        math::Vector start;
        math::Vector end;
    };

    struct PolygonData
    {
        mono::Color::RGBA fill_color;
        mono::Color::RGBA outline_color;
        std::vector<math::Vector> vertices;
    };

    struct DotData
    {
        mono::Color::RGBA color;
        math::Vector position;
    };

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
        circle_data.outline_color = mono::Color::RGBA(outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);
        circle_data.fill_color = mono::Color::RGBA(fillColor.r, fillColor.g, fillColor.b, fillColor.a);

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
            polygon_data.vertices.emplace_back(vertex.x, vertex.y);
        }

        polygon_data.outline_color = mono::Color::RGBA(outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);
        polygon_data.fill_color = mono::Color::RGBA(fillColor.r, fillColor.g, fillColor.b, fillColor.a);

        DebugDrawCollection* debug_collection = static_cast<DebugDrawCollection*>(data);
        debug_collection->polygons.push_back(polygon_data);
    }

    void DebugDrawDot(cpFloat size, cpVect pos, cpSpaceDebugColor color, cpDataPointer data)
    {
        //DotData dot_data;
        //dot_data.position = math::Vector(pos.x, pos.y);
        //dot_data.color = mono::Color::RGBA(color.r, color.g, color.b, color.a)

        DebugDrawCollection* debug_collection = static_cast<DebugDrawCollection*>(data);
        debug_collection->dots.push_back(math::Vector(pos.x, pos.y));
    }
    
    cpSpaceDebugColor DebugDrawColorForShape(cpShape *shape, cpDataPointer data)
    {
        const cpBodyType body_type = cpBodyGetType(cpShapeGetBody(shape));
        return (body_type == CP_BODY_TYPE_STATIC) ? ConvertColor(mono::Color::BLUE) : ConvertColor(mono::Color::RED);
    }
}

static_assert(uint32_t(CP_SPACE_DEBUG_DRAW_SHAPES) == mono::PhysicsDebugComponents::DRAW_SHAPES);
static_assert(uint32_t(CP_SPACE_DEBUG_DRAW_CONSTRAINTS) == mono::PhysicsDebugComponents::DRAW_CONSTRAINTS);
static_assert(uint32_t(CP_SPACE_DEBUG_DRAW_COLLISION_POINTS) == mono::PhysicsDebugComponents::DRAW_COLLISION_POINTS);

PhysicsDebugDrawer::PhysicsDebugDrawer(
    const bool& enabled_drawing,
    const bool& enabled_interaction,
    const bool& enabled_body_introspection,
    const uint32_t& debug_components,
    mono::PhysicsSystem* physics_system,
    mono::EventHandler* event_handler)
    : m_enabled_drawing(enabled_drawing)
    , m_enabled_interaction(enabled_interaction)
    , m_enabled_body_introspection(enabled_body_introspection)
    , m_debug_components(debug_components)
    , m_physics_system(physics_system)
    , m_event_handler(event_handler)
    , m_mouse_down(false)
    , m_shift_down(false)
    , m_body_id(0)
{
    using namespace std::placeholders;

    event::MouseDownEventFunc mouse_down = std::bind(&PhysicsDebugDrawer::OnMouseDown, this, _1);
    event::MouseUpEventFunc mouse_up = std::bind(&PhysicsDebugDrawer::OnMouseUp, this, _1);
    event::MouseMotionEventFunc mouse_move = std::bind(&PhysicsDebugDrawer::OnMouseMove, this, _1);

    event::KeyDownEventFunc key_down = std::bind(&PhysicsDebugDrawer::OnKeyDown, this, _1);
    event::KeyUpEventFunc key_up = std::bind(&PhysicsDebugDrawer::OnKeyUp, this, _1);

    m_mouse_down_token = m_event_handler->AddListener(mouse_down);
    m_mouse_up_token = m_event_handler->AddListener(mouse_up);
    m_mouse_move_token = m_event_handler->AddListener(mouse_move);

    m_key_down_token = m_event_handler->AddListener(key_down);
    m_key_up_token = m_event_handler->AddListener(key_up);

    m_click_timestamp = std::numeric_limits<uint32_t>::max();
}

PhysicsDebugDrawer::~PhysicsDebugDrawer()
{
    m_event_handler->RemoveListener(m_mouse_down_token);
    m_event_handler->RemoveListener(m_mouse_up_token);
    m_event_handler->RemoveListener(m_mouse_move_token);

    m_event_handler->RemoveListener(m_key_down_token);
    m_event_handler->RemoveListener(m_key_up_token);
}

void PhysicsDebugDrawer::Draw(mono::IRenderer& renderer) const
{
    if(m_enabled_body_introspection)
        DrawBodyIntrospection(renderer);

    if(!m_enabled_drawing)
        return;

    DrawPhysics(renderer);

    if(m_debug_components & mono::PhysicsDebugComponents::DRAW_BODY_FORCES)
        DrawForces(renderer);

    if(m_click_timestamp != std::numeric_limits<uint32_t>::max())
    {
        renderer.DrawCircle(m_mouse_down_position, 0.25f, 6, 2.0f, mono::Color::RED);
        renderer.DrawCircle(m_mouse_up_position, 0.5f, 6, 2.0f, mono::Color::RED);
        renderer.DrawPoints(m_found_positions, mono::Color::BLUE, 16.0f);

        const uint32_t diff = System::GetMilliseconds() - m_click_timestamp;
        if(diff > 5000)
            m_click_timestamp = std::numeric_limits<uint32_t>::max();
    }

    if(m_mouse_down)
    {
        if(m_shift_down)
        {
            const math::Quad world_bb(m_mouse_down_position, m_mouse_position);
            renderer.DrawQuad(world_bb, mono::Color::GREEN, 2.0f);
        }
        else
        {
            renderer.DrawLines({ m_mouse_down_position, m_mouse_position }, mono::Color::GREEN, 2.0f);
        }
    }
}

math::Quad PhysicsDebugDrawer::BoundingBox() const
{
    return math::InfQuad;
}

void PhysicsDebugDrawer::DrawBodyIntrospection(mono::IRenderer& renderer) const
{
    ImGui::SetNextWindowSize(ImVec2(1100, 200));
    const bool window_open = ImGui::Begin("Body Introspection");
    if(!window_open)
        return;

    ImGui::InputInt("Body Id", &m_body_id);
    ImGui::Separator();
    ImGui::Spacing();

    const bool valid_body_id = m_physics_system->IsAllocated(m_body_id);
    if(valid_body_id)
    {
        const mono::IBody* body = m_physics_system->GetBody(m_body_id);
        const mono::BodyType body_type = body->GetType();

        const char* type_string = nullptr;

        if(body_type == mono::BodyType::DYNAMIC)
            type_string = "Dynamic";
        else if(body_type == mono::BodyType::KINEMATIC)
            type_string = "Kinematic";
        else if(body_type == mono::BodyType::STATIC)
            type_string = "Static";

        cpBody* body_native_handle = body->Handle();

        ImGui::TextDisabled("BODY");

        const bool table_success = ImGui::BeginTable("body_table", 13, ImGuiTableFlags_BordersInnerV);
        if(table_success)
        {
            ImGui::TableSetupColumn("Type");
            ImGui::TableSetupColumn("Mass");
            ImGui::TableSetupColumn("Mass_I");
            ImGui::TableSetupColumn("Inertia");
            ImGui::TableSetupColumn("Inertia_I");
            ImGui::TableSetupColumn("COG");
            ImGui::TableSetupColumn("Position");
            ImGui::TableSetupColumn("Velocity");
            ImGui::TableSetupColumn("Force");
            ImGui::TableSetupColumn("Angle");
            ImGui::TableSetupColumn("Angular V");
            ImGui::TableSetupColumn("Torque");
            ImGui::TableSetupColumn("Velocity Bias");
            ImGui::TableHeadersRow();

            ImGui::TableNextRow();

            ImGui::TableNextColumn(); ImGui::Text("%s", type_string);
            ImGui::TableNextColumn(); ImGui::Text("%f", body_native_handle->m);
            ImGui::TableNextColumn(); ImGui::Text("%f", body_native_handle->m_inv);
            ImGui::TableNextColumn(); ImGui::Text("%f", body_native_handle->i);
            ImGui::TableNextColumn(); ImGui::Text("%f", body_native_handle->i_inv);
            ImGui::TableNextColumn(); ImGui::Text("%.2f, %.2f", body_native_handle->cog.x, body_native_handle->cog.y);
            ImGui::TableNextColumn(); ImGui::Text("%.2f, %.2f", body_native_handle->p.x, body_native_handle->p.y);
            ImGui::TableNextColumn(); ImGui::Text("%.2f (%.2f, %.2f)", cpvlength(body_native_handle->v), body_native_handle->v.x, body_native_handle->v.y);
            ImGui::TableNextColumn(); ImGui::Text("%.2f, %.2f", body_native_handle->f.x, body_native_handle->f.y);
            ImGui::TableNextColumn(); ImGui::Text("%f", body_native_handle->a);
            ImGui::TableNextColumn(); ImGui::Text("%f", body_native_handle->w);
            ImGui::TableNextColumn(); ImGui::Text("%f", body_native_handle->t);
            ImGui::TableNextColumn(); ImGui::Text("%.2f, %.2f", body_native_handle->v_bias.x, body_native_handle->v_bias.y);

            ImGui::EndTable();
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::TextDisabled("SHAPES");

        const bool shapes_table_success = ImGui::BeginTable("shapes_table", 6, ImGuiTableFlags_BordersInnerV);
        if(shapes_table_success)
        {
            ImGui::TableSetupColumn("Sensor");
            ImGui::TableSetupColumn("Mass");
            ImGui::TableSetupColumn("Inertia");
            ImGui::TableSetupColumn("Group");
            ImGui::TableSetupColumn("Categories");
            ImGui::TableSetupColumn("Mask");
            ImGui::TableHeadersRow();

            const std::vector<mono::IShape*>& shapes = m_physics_system->GetShapesAttachedToBody(m_body_id);
            for(size_t index = 0; index < shapes.size(); ++index)
            {
                const mono::IShape* shape = shapes[index];
                const cpShape* shape_native_handle = shape->Handle();

                ImGui::TableNextRow();
                ImGui::TableNextColumn(); ImGui::Text("%s", shape_native_handle->sensor ? "Yes" : "No");
                ImGui::TableNextColumn(); ImGui::Text("%f", shape_native_handle->massInfo.m);
                ImGui::TableNextColumn(); ImGui::Text("%f", shape_native_handle->massInfo.i);

                const cpShapeFilter filter = shape_native_handle->filter;
                ImGui::TableNextColumn(); ImGui::Text("%lu", filter.group);
                ImGui::TableNextColumn(); ImGui::Text("%u", filter.categories);
                ImGui::TableNextColumn(); ImGui::Text("%u", filter.mask);
            }

            ImGui::EndTable();
        }

        ImGui::Spacing();
        ImGui::Separator();
    }
    else
    {
        ImGui::TextDisabled("Invalid body id...");
    }

    ImGui::End();
}

void PhysicsDebugDrawer::DrawPhysics(const mono::IRenderer& renderer) const
{
    DebugDrawCollection debug_collection;

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

    mono::PhysicsSpace* physics_space = m_physics_system->GetSpace();
    cpSpaceDebugDraw(physics_space->Handle(), &options);

    if(!debug_collection.dots.empty())
        renderer.DrawPoints(debug_collection.dots, mono::Color::GREEN, 2.0f);

    for(const CircleData& circle_data : debug_collection.circles)
        renderer.DrawCircle(circle_data.position, circle_data.radius, 16, 1.0f, circle_data.fill_color);

    if(!debug_collection.segments.empty())
        renderer.DrawLines(debug_collection.segments, mono::Color::MAGENTA, 1.0f);

    if(!debug_collection.fat_segments.empty())
        renderer.DrawLines(debug_collection.fat_segments, mono::Color::MAGENTA, 4.0f);

    for(const PolygonData& polygon_data : debug_collection.polygons)
        renderer.DrawClosedPolyline(polygon_data.vertices, polygon_data.fill_color, 2.0f);    
}

void PhysicsDebugDrawer::DrawForces(mono::IRenderer& renderer) const
{
    std::vector<math::Vector> lines;

    char text_buffer[256] = { 0 };

    const mono::ForEachBodyFunc body_func = [&renderer, &lines, &text_buffer](uint32_t id, mono::IBody& body) {

        if(body.GetType() == mono::BodyType::STATIC)
            return;

        const math::Vector& world_position = body.GetPosition();
        const mono::CullResult cull_result = renderer.Cull(math::Quad(world_position, 0.5f));
        if(cull_result != mono::CullResult::IN_VIEW)
            return;

        const math::Vector& velocity = body.GetVelocity();
        lines.push_back(world_position);
        lines.push_back(world_position + velocity);

        const float speed = math::Length(velocity);

        const auto transform_scope = mono::MakeTransformScope(math::CreateMatrixWithPosition(world_position), &renderer);
        std::snprintf(text_buffer, std::size(text_buffer), "%.2f", speed);
        renderer.RenderText(0, text_buffer, mono::Color::CYAN, mono::FontCentering::DEFAULT_CENTER);
    };
    m_physics_system->ForEachBody(body_func);

    renderer.DrawLines(lines, mono::Color::CYAN, 1.0f);
}

mono::EventResult PhysicsDebugDrawer::OnMouseDown(const event::MouseDownEvent& event)
{
    if(!m_enabled_interaction)
        return mono::EventResult::PASS_ON;

    m_mouse_down = true;
    m_mouse_down_position = math::Vector(event.world_x, event.world_y);
    return mono::EventResult::HANDLED;
}

mono::EventResult PhysicsDebugDrawer::OnMouseUp(const event::MouseUpEvent& event)
{
    m_mouse_down = false;

    if(!m_enabled_interaction)
        return mono::EventResult::PASS_ON;

    m_found_positions.clear();

    m_mouse_up_position = math::Vector(event.world_x, event.world_y);
    m_click_timestamp = System::GetMilliseconds();

    mono::PhysicsSpace* physics_space = m_physics_system->GetSpace();
    const bool same_point = math::IsPrettyMuchEquals(m_mouse_up_position, m_mouse_down_position);
    if(same_point)
    {
        const mono::QueryResult query_result = physics_space->QueryNearest(m_mouse_up_position, 10.0f, -1);
        if(query_result.body)
            m_found_positions.push_back(query_result.body->GetPosition());
    }
    else if(m_shift_down)
    {
        const math::Quad world_bb(m_mouse_down_position, m_mouse_up_position);
        const std::vector<mono::QueryResult> found_bodies = physics_space->QueryBox(world_bb, -1);
        for(const mono::QueryResult& query_result : found_bodies)
            m_found_positions.push_back(query_result.body->GetPosition());
    }
    else
    {
        const std::vector<mono::QueryResult> found_bodies
            = physics_space->QueryAllInLIne(m_mouse_down_position, m_mouse_up_position, 1.0f, -1);
        for(const mono::QueryResult& query_result : found_bodies)
            m_found_positions.push_back(query_result.body->GetPosition());
    }

    return mono::EventResult::HANDLED;
}

mono::EventResult PhysicsDebugDrawer::OnMouseMove(const event::MouseMotionEvent& event)
{
    m_mouse_position = math::Vector(event.world_x, event.world_y);
    return mono::EventResult::PASS_ON;
}

mono::EventResult PhysicsDebugDrawer::OnKeyDown(const event::KeyDownEvent& event)
{
    m_shift_down = event.shift;
    return mono::EventResult::PASS_ON;
}

mono::EventResult PhysicsDebugDrawer::OnKeyUp(const event::KeyUpEvent& event)
{
    m_shift_down = event.shift;
    return mono::EventResult::PASS_ON;
}

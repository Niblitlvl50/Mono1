
#include "PathTool.h"
#include "Editor.h"
#include "IRenderer.h"
#include "Color.h"

using namespace editor;

class PathTool::Visualizer : public mono::IDrawable
{
public:

    Visualizer(const std::vector<math::Vector2f>& points, const math::Vector2f& mouse_position)
        : m_points(points),
          m_mousePosition(mouse_position)
    { }

    virtual void doDraw(mono::IRenderer& renderer) const
    {
        if(m_points.empty())
            return;

        constexpr mono::Color::RGBA line_color(1.0f, 0.0f, 0.0f, 0.2f);
        constexpr mono::Color::RGBA point_color(1.0f, 0.0f, 1.0f, 0.8f);
        
        const std::vector<math::Vector2f>& line = { m_points.back(), m_mousePosition };
        
        renderer.DrawPolyline(m_points, line_color, 2.0f);
        renderer.DrawLines(line, line_color, 2.0f);
        renderer.DrawPoints(m_points, point_color, 4.0f);
    }

    virtual math::Quad BoundingBox() const
    {
        return math::Quad(-math::INF, -math::INF, math::INF, math::INF);
    }

    const std::vector<math::Vector2f>& m_points;
    const math::Vector2f& m_mousePosition;
};

PathTool::PathTool(Editor* editor)
    : m_editor(editor),
      m_active(false),
      m_visualizer(std::make_shared<Visualizer>(m_points, m_mousePosition))
{ }

void PathTool::Begin()
{
    m_active = true;
    m_editor->AddDrawable(m_visualizer, 3);
}

void PathTool::End()
{
    m_editor->AddPath(m_points);
    m_editor->RemoveDrawable(m_visualizer);
    m_points.clear();

    m_active = false;
}

bool PathTool::IsActive() const
{
    return m_active;
}

void PathTool::HandleMouseDown(const math::Vector2f& world_pos, mono::IEntityPtr entity)
{ }

void PathTool::HandleMouseUp(const math::Vector2f& world_pos)
{
    m_points.push_back(world_pos);
}

void PathTool::HandleMousePosition(const math::Vector2f& world_pos)
{
    m_mousePosition = world_pos;
}

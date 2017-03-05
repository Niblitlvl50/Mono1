
#include "PathTool.h"
#include "Editor.h"
#include "IRenderer.h"
#include "Color.h"
#include "Path.h"

using namespace editor;

class PathTool::Visualizer : public mono::IDrawable
{
public:

    Visualizer(const std::vector<math::Vector>& points, const math::Vector& mouse_position)
        : m_points(points),
          m_mousePosition(mouse_position)
    { }

    virtual void doDraw(mono::IRenderer& renderer) const
    {
        if(m_points.empty())
            return;

        constexpr mono::Color::RGBA line_color(1.0f, 0.0f, 0.0f, 0.2f);

        DrawPath(renderer, m_points);

        const std::vector<math::Vector>& line = { m_points.back(), m_mousePosition };
        renderer.DrawLines(line, line_color, 2.0f);
    }

    virtual math::Quad BoundingBox() const
    {
        return math::Quad(-math::INF, -math::INF, math::INF, math::INF);
    }

    const std::vector<math::Vector>& m_points;
    const math::Vector& m_mousePosition;
};

PathTool::PathTool(Editor* editor)
    : m_editor(editor),
      m_visualizer(std::make_shared<Visualizer>(m_points, m_mousePosition))
{ }

void PathTool::Begin()
{
    m_editor->AddDrawable(m_visualizer, 0);
}

void PathTool::End()
{
    m_editor->RemoveDrawable(m_visualizer);
}

bool PathTool::IsActive() const
{
    return true;
}

void PathTool::HandleContextMenu(int menu_index)
{
    if(m_points.empty())
        return;

    if(menu_index == 0)
    {
        m_editor->AddPath(std::make_shared<editor::PathEntity>("New path", m_points));
        m_points.clear();
    }
    else if(menu_index == 1)
    {
        m_points.pop_back();
    }
}

void PathTool::HandleMouseDown(const math::Vector& world_pos, mono::IEntityPtr entity)
{ }

void PathTool::HandleMouseUp(const math::Vector& world_pos)
{
    m_points.push_back(world_pos);
}

void PathTool::HandleMousePosition(const math::Vector& world_pos)
{
    m_mousePosition = world_pos;
}

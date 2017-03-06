
#include "PolygonTool.h"
#include "Editor.h"
#include "Polygon.h"
#include "IRenderer.h"
#include "Color.h"
#include "Math/Quad.h"

#include "Texture/TextureFactory.h"

using namespace editor;

class PolygonTool::Visualizer : public mono::IDrawable
{
public:

    Visualizer(const std::vector<math::Vector>& points, const math::Vector& mouse_position)
        : m_points(points),
          m_mousePosition(mouse_position)
    {
        m_texture = mono::CreateTexture("textures/placeholder.png");
    }

    virtual void doDraw(mono::IRenderer& renderer) const
    {
        if(m_points.empty())
            return;

        std::vector<math::Vector> texture_points;
        texture_points.resize(m_points.size());

        DrawPolygon(renderer, m_texture, m_points, texture_points);

        constexpr mono::Color::RGBA color(1.0f, 0.0f, 0.0f, 0.2f);
        const std::vector<math::Vector>& line = { m_points.back(), m_mousePosition, m_points.front() };
        renderer.DrawPolyline(line, color, 2.0f);
    }

    virtual math::Quad BoundingBox() const
    {
        return math::Quad(-math::INF, -math::INF, math::INF, math::INF);
    }

    const std::vector<math::Vector>& m_points;
    const math::Vector& m_mousePosition;

    mono::ITexturePtr m_texture;
};

PolygonTool::PolygonTool(Editor* editor)
    : m_editor(editor)
{
    m_visualizer = std::make_shared<Visualizer>(m_points, m_mousePosition);
}

void PolygonTool::Begin()
{
    m_editor->AddDrawable(m_visualizer, 1);
}

void PolygonTool::End()
{
    m_editor->RemoveDrawable(m_visualizer);
}

bool PolygonTool::IsActive() const
{
    return true;
}

void PolygonTool::HandleContextMenu(int menu_index)
{
    if(m_points.empty())
        return;

    if(menu_index == 0)
    {
        m_editor->AddPolygon(std::make_shared<editor::PolygonEntity>(m_points));
        m_points.clear();
    }
    else if(menu_index == 1)
    {
        m_points.pop_back();
    }
}

void PolygonTool::HandleMouseDown(const math::Vector& world_pos, mono::IEntityPtr entity)
{ }

void PolygonTool::HandleMouseUp(const math::Vector& world_pos)
{
    m_points.push_back(world_pos);
}

void PolygonTool::HandleMousePosition(const math::Vector& world_pos)
{
    m_mousePosition = world_pos;
}

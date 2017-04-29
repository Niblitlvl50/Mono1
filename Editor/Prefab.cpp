
#include "Prefab.h"
#include "SnapPoint.h"
#include "Sprite/ISprite.h"
#include "Sprite/SpriteFactory.h"
#include "Rendering/IRenderer.h"
#include "Rendering/Color.h"
#include "Math/Quad.h"

#include <cmath>

using namespace editor;

Prefab::Prefab(const std::string& name, const std::string& sprite_file, const std::vector<SnapPoint>& snap_points)
    : m_name(name),
      m_snap_points(snap_points),
      m_selected(false)
{
    m_sprite = mono::CreateSprite(sprite_file.c_str());
}

void Prefab::Draw(mono::IRenderer& renderer) const
{
    if(m_selected)
    {
        math::Quad bb(-0.5f, -0.5f, 0.5f, 0.5f);
        renderer.DrawQuad(bb, mono::Color::RGBA(0.0f, 1.0f, 0.0f), 2.0f);
    }

    renderer.DrawSprite(*m_sprite);

    std::vector<math::Vector> points;
    points.reserve(m_snap_points.size() * 2);

    for(const SnapPoint& snap_point : m_snap_points)
    {
        points.emplace_back(snap_point.position);

        const float x = std::sin(snap_point.normal);
        const float y = std::cos(snap_point.normal);

        points.emplace_back(snap_point.position + math::Vector(x, y) * 0.5f);
    }

    constexpr mono::Color::RGBA color(1.0f, 0.0f, 0.0f);
    renderer.DrawPoints(points, color, 4.0f);
    renderer.DrawLines(points, color, 1.0f);
}

void Prefab::Update(unsigned int delta)
{
    m_sprite->doUpdate(delta);
}

const std::string& Prefab::Name() const
{
    return m_name;
}

void Prefab::SetSelected(bool selected)
{
    m_selected = selected;
}

const std::vector<SnapPoint>& Prefab::SnapPoints() const
{
    return m_snap_points;
}

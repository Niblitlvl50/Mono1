
#include "GrabberVisualizer.h"
#include "Math/MathFwd.h"
#include "Math/Quad.h"
#include "Grabber.h"
#include "Color.h"
#include "IRenderer.h"

using namespace editor;

GrabberVisualizer::GrabberVisualizer(const std::vector<editor::Grabber>& grabbers)
    : m_grabbers(grabbers)
{ }

void GrabberVisualizer::doDraw(mono::IRenderer& renderer) const
{
    auto transform_func = [](const editor::Grabber& grabber) {
        return grabber.position;
    };

    std::vector<math::Vector2f> points;
    points.resize(m_grabbers.size());
    std::transform(m_grabbers.begin(), m_grabbers.end(), points.begin(), transform_func);

    constexpr mono::Color::RGBA default_color(1.0f, 0.5f, 0.0f, 0.8f);
    constexpr mono::Color::RGBA hoover_color(0.0f, 0.5f, 1.0f, 1.0f);

    renderer.DrawPoints(points, default_color, 10.0f);

    for(const auto& grabber : m_grabbers)
    {
        if(grabber.hoover)
        {
            renderer.DrawPoints({ grabber.position }, hoover_color, 10.0f);
        }
    }
}

math::Quad GrabberVisualizer::BoundingBox() const
{
    return math::Quad(-math::INF, -math::INF, math::INF, math::INF);
}

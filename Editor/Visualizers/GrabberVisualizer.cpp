
#include "GrabberVisualizer.h"
#include "Math/MathFwd.h"
#include "Math/Quad.h"
#include "Grabber.h"
#include "Rendering/Color.h"
#include "Rendering/IRenderer.h"

using namespace editor;

GrabberVisualizer::GrabberVisualizer(const std::vector<editor::Grabber>& grabbers)
    : m_grabbers(grabbers)
{ }

void GrabberVisualizer::doDraw(mono::IRenderer& renderer) const
{
    std::vector<math::Vector> points;
    points.reserve(m_grabbers.size());

    std::vector<math::Vector> hoovered_points;
    hoovered_points.reserve(m_grabbers.size());

    for(const Grabber& grabber : m_grabbers)
    {
        if(grabber.hoover)
            hoovered_points.push_back(grabber.position);
        else
            points.push_back(grabber.position);
    }

    constexpr mono::Color::RGBA default_color(1.0f, 0.5f, 0.0f, 0.8f);
    constexpr mono::Color::RGBA hoover_color(0.0f, 0.5f, 1.0f, 1.0f);

    renderer.DrawPoints(points, default_color, 10.0f);
    renderer.DrawPoints(hoovered_points, hoover_color, 10.0f);
}

math::Quad GrabberVisualizer::BoundingBox() const
{
    return math::Quad(-math::INF, -math::INF, math::INF, math::INF);
}

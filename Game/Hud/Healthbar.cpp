
#include "Healthbar.h"
#include "Rendering/IRenderer.h"
#include "Rendering/Color.h"
#include "Math/Quad.h"

using namespace game;

namespace
{
    std::vector<math::Vector> GenerateHealthbarVertices(const std::vector<Healthbar>& healthbars, bool full_width)
    {
        std::vector<math::Vector> vertices;
        vertices.reserve(healthbars.size() * 2);

        for(const Healthbar& bar : healthbars)
        {
            const math::Vector shift_vector(bar.width / 2.0f, 0.0f);

            const math::Vector& left = bar.position - shift_vector;
            const math::Vector& right = bar.position + shift_vector;
            const math::Vector& diff = right - left;

            const float percentage = full_width ? 1.0f : bar.health_percentage;

            vertices.emplace_back(left);
            vertices.emplace_back(left + (diff * percentage));
        }

        return vertices;
    }
}

HealthbarDrawer::HealthbarDrawer(const std::vector<Healthbar>& healthbars)
    : m_healthbars(healthbars)
{ }

void HealthbarDrawer::doDraw(mono::IRenderer& renderer) const
{
    constexpr float line_width = 4.0f;
    constexpr mono::Color::RGBA background_color(0.5f, 0.5f, 0.5f, 1.5f);
    constexpr mono::Color::RGBA healthbar_color(1.0f, 0.3f, 0.3f, 1.0f);

    const std::vector<math::Vector>& background_lines = GenerateHealthbarVertices(m_healthbars, true);
    const std::vector<math::Vector>& healthbar_lines = GenerateHealthbarVertices(m_healthbars, false);

    renderer.DrawLines(background_lines, background_color, line_width);
    renderer.DrawLines(healthbar_lines, healthbar_color, line_width);
}

math::Quad HealthbarDrawer::BoundingBox() const
{
    return math::Quad(-math::INF, -math::INF, math::INF, math::INF);
}


#include "ScreenFadeDrawer.h"
#include "RenderSystem.h"
#include "IRenderer.h"
#include "Math/Quad.h"

using namespace mono;

ScreenFadeDrawer::ScreenFadeDrawer(const RenderSystem* render_system)
    : m_render_system(render_system)
{ }
void ScreenFadeDrawer::Draw(mono::IRenderer& renderer) const
{
    const bool should_apply_fade = m_render_system->ShouldApplyScreenFadeAlpha();
    if(should_apply_fade)
        renderer.SetScreenFadeAlpha(m_render_system->GetFadeAlpha());
}
math::Quad ScreenFadeDrawer::BoundingBox() const
{
    return math::InfQuad;
}

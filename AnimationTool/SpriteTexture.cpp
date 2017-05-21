
#include "SpriteTexture.h"
#include "Math/Quad.h"
#include "Math/Matrix.h"
#include "Rendering/IRenderer.h"
#include "Rendering/Color.h"

using namespace animator;

SpriteTexture::SpriteTexture(const mono::ITexturePtr& texture, const math::Quad& full_texture)
{
    m_texture.Init(texture, { full_texture });
    m_texture.DefineAnimation("none", { 0, 100 }, true);

    m_width = full_texture.mB.x - full_texture.mA.x;
    m_height = full_texture.mA.y - full_texture.mB.y;
}

void SpriteTexture::Draw(mono::IRenderer& renderer) const
{
    if(!m_enabled)
        return;

    const float ratio = m_width / m_height;
    const math::Vector scale(100 * ratio, 100);

    math::Matrix scale_matrix;
    math::ScaleXY(scale_matrix, scale);

    math::Matrix translate_matrix;
    math::Translate(translate_matrix, math::Vector(scale.x / 2 + 10, 320 - scale.y / 2 - 10));

    const math::Matrix& projection = math::Ortho(0.0f, 480, 0.0f, 320.0f, -10.0f, 10.0f);
    const math::Matrix& model_view = translate_matrix * scale_matrix;

    renderer.PushNewProjection(projection);
    renderer.PushNewTransform(model_view);

    math::Quad bb(-0.5, -0.5, 0.5, 0.5);
    renderer.DrawQuad(bb, mono::Color::RGBA(0, 0, 0), 2.0f);
    renderer.DrawSprite(m_texture);
}

void SpriteTexture::Update(unsigned int delta)
{ }

void SpriteTexture::Enable(bool enable)
{
    m_enabled = enable;
}

bool SpriteTexture::Enabled() const
{
    return m_enabled;
}

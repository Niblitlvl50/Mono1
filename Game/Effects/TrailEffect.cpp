
#include "TrailEffect.h"
#include "Particle/ParticlePool.h"
#include "Particle/ParticleEmitter.h"
#include "Particle/ParticleSystemDefaults.h"
#include "Rendering/Texture/TextureFactory.h"
#include "Rendering/IRenderer.h"
#include "Math/Matrix.h"
#include "Math/Quad.h"

using namespace game;

namespace
{
    void TrailGenerator(const math::Vector& position, mono::ParticlePool& pool, size_t index)
    {
        constexpr int life = 500;

        pool.m_position[index] = position;
        pool.m_startColor[index] = mono::Color::RGBA(1.0f, 0.0f, 0.0f, 1.0f);
        pool.m_endColor[index] = mono::Color::RGBA(1.0f, 0.0f, 0.0f, 0.1f);
        pool.m_startLife[index] = life;
        pool.m_life[index] = life;
    }
}

TrailEffect::TrailEffect(const math::Vector& position)
    : m_position(position)
{
    mono::ParticleEmitter::Configuration config;
    //config.position = position;
    config.generator = TrailGenerator;
    config.updater = mono::DefaultUpdater;
    config.texture = mono::CreateTexture("textures/flare.png");
    config.emit_rate = 0.2f;
    config.point_size = 16.0f;

    m_pool = std::make_unique<mono::ParticlePool>(1000);
    m_emitter = std::make_unique<mono::ParticleEmitter>(config, *m_pool.get());
}

TrailEffect::~TrailEffect()
{ }

void TrailEffect::Draw(mono::IRenderer& renderer) const
{
    renderer.PushGlobalTransform();
    m_emitter->doDraw(renderer);
}

void TrailEffect::Update(unsigned int delta)
{
    m_emitter->doUpdate(delta);
    m_emitter->SetPosition(m_position);
}

math::Quad TrailEffect::BoundingBox() const
{
    constexpr float inf = std::numeric_limits<float>::infinity();
    return math::Quad(-inf, -inf, inf, inf);
}

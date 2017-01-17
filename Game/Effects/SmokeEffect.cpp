
#include "SmokeEffect.h"
#include "Particle/ParticleEmitter.h"
#include "Particle/ParticleSystemDefaults.h"
#include "Texture/TextureFactory.h"

using namespace game;

SmokeEffect::SmokeEffect(const math::Vector& position)
    : m_pool(100000)
{
    mPosition = position;

    mono::ParticleEmitter::Configuration config;
    config.position = position;
    config.generator = mono::DefaultGenerator;
    config.updater = mono::DefaultUpdater;
    config.texture = mono::CreateTexture("textures/flare.png");
    config.emit_rate = 1.0f;
    config.point_size = 32.0f;

    m_emitter = std::make_shared<mono::ParticleEmitter>(config, m_pool);
}

void SmokeEffect::Draw(mono::IRenderer& renderer) const
{
    m_emitter->doDraw(renderer);
}

void SmokeEffect::Update(unsigned int delta)
{
    m_emitter->doUpdate(delta);
}

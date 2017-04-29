
#include "SmokeEffect.h"
#include "Particle/ParticlePool.h"
#include "Particle/ParticleEmitter.h"
#include "Particle/ParticleSystemDefaults.h"
#include "Rendering/Texture/TextureFactory.h"
#include "Random.h"

#include "Math/MathFunctions.h"

using namespace game;

namespace
{
    void SmokeGenerator(const math::Vector& position, mono::ParticlePool& pool, size_t index)
    {
        const float x = mono::Random(-2.0f, 2.0f);
        const float y = mono::Random(-2.0f, 2.0f);

        const int life = mono::RandomInt(0, 500) + 2000;

        pool.m_position[index] = position;
        pool.m_velocity[index] = math::Vector(x, y);
        pool.m_startColor[index] = mono::Color::RGBA(1.0f, 0.0f, 0.0f, 1.0f);
        pool.m_endColor[index] = mono::Color::RGBA(0.0f, 1.0f, 0.0f, 0.1f);
        pool.m_startLife[index] = life;
        pool.m_life[index] = life;
    }
}

SmokeEffect::SmokeEffect(const math::Vector& position)
{
    mPosition = position;

    mono::ParticleEmitter::Configuration config;
    //config.position = position;
    config.generator = SmokeGenerator;
    config.updater = mono::DefaultUpdater;
    config.texture = mono::CreateTexture("textures/smoke.png");
    config.emit_rate = 0.1f;
    config.point_size = 64.0f;

    m_pool = std::make_unique<mono::ParticlePool>(1000);
    m_emitter = std::make_unique<mono::ParticleEmitter>(config, *m_pool.get());
}

SmokeEffect::~SmokeEffect()
{ }

void SmokeEffect::Draw(mono::IRenderer& renderer) const
{
    m_emitter->doDraw(renderer);
}

void SmokeEffect::Update(unsigned int delta)
{
    m_emitter->doUpdate(delta);
    //mRotation += math::ToRadians(0.1) * float(delta);
}


#pragma once

#include "Math/MathFwd.h"
#include <cstdint>
#include <functional>

namespace mono
{
    class ParticleSystem;
    struct ParticlePoolComponent;
    struct ParticlePoolComponentView;
    struct ParticleEmitterComponent;

    using ParticleGenerator = std::function<void (const math::Vector& position, ParticlePoolComponentView& component_view)>;
    using ParticleUpdater   = std::function<void (ParticlePoolComponent& pool, uint32_t count, uint32_t delta_ms)>;

    void DefaultGenerator(const math::Vector& position, struct ParticlePoolComponentView& particle_view);
    void DefaultUpdater(ParticlePoolComponent& pool, uint32_t count, uint32_t delta_ms);

}

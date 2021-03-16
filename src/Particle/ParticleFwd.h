
#pragma once

#include "Math/MathFwd.h"
#include <stdlib.h>
#include <cstdint>
#include <functional>

namespace mono
{
    class ParticleSystem;
    struct ParticlePoolComponent;
    struct ParticleEmitterComponent;

    using ParticleGenerator = std::function<void (const math::Vector& position, ParticlePoolComponent& pool, size_t index)>;
    using ParticleUpdater   = std::function<void (ParticlePoolComponent& pool, size_t count, uint32_t delta_ms)>;

    void DefaultGenerator(const math::Vector& position, ParticlePoolComponent& pool, size_t index);
    void DefaultUpdater(ParticlePoolComponent& pool, size_t count, uint32_t delta_ms);
}

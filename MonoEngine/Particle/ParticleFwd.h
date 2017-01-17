
#pragma once

#include "Math/MathFwd.h"
#include <cstddef>

namespace mono
{
    class ParticlePool;
    class ParticleEmitter;

    using ParticleGenerator = void (*)(const math::Vector& position, ParticlePool& pool, size_t index);
    using ParticleUpdater   = void (*)(ParticlePool& pool, size_t count, unsigned int delta);
}
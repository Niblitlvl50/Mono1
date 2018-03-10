
#pragma once

#include "Math/MathFwd.h"
#include <stdlib.h>

namespace mono
{
    class ParticlePool;
    class ParticleEmitter;
    class ParticleDrawer;

    using ParticleGenerator = void (*)(const math::Vector& position, ParticlePool& pool, size_t index, const void* context);
    using ParticleUpdater   = void (*)(ParticlePool& pool, size_t count, unsigned int delta, const void* context);
}
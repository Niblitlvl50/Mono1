
#pragma once

#include "Math/MathFwd.h"
#include <stdlib.h>
#include <functional>

namespace mono
{
    class ParticlePool;
    class ParticleEmitter;
    class ParticleDrawer;

    using ParticleGenerator = std::function<void (const math::Vector& position, ParticlePool& pool, size_t index)>;
    using ParticleUpdater   = std::function<void (ParticlePool& pool, size_t count, unsigned int delta)>;
}
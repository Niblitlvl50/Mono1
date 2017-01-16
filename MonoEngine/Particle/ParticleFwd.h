
#pragma once

#include <cstddef>

namespace mono
{
    class ParticlePool;

    using ParticleGenerator = void (*)(ParticlePool& pool, size_t index);
    using ParticleUpdater   = void (*)(ParticlePool& pool, size_t count, unsigned int delta);
}

#pragma once

#include "ParticleFwd.h"

namespace mono
{
    void DefaultGenerator(ParticlePool& pool, size_t index);
    void DefaultUpdater(ParticlePool& pool, size_t count, unsigned int delta);
}

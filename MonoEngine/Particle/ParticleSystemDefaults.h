
#pragma once

#include "ParticleFwd.h"

namespace mono
{
    void DefaultGenerator(const math::Vector& position, ParticlePool& pool, size_t index, const void* context);
    void DefaultUpdater(ParticlePool& pool, size_t count, unsigned int delta, const void* context);
}

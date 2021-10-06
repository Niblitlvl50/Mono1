
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
    using ParticleUpdater   = std::function<void (ParticlePoolComponentView& component_view, float delta_s)>;

    void DefaultGenerator(const math::Vector& position, ParticlePoolComponentView& particle_view);
    void DefaultUpdater(ParticlePoolComponentView& component_view, float delta_s);

}

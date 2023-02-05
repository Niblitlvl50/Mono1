
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
    struct ParticleGeneratorContext;

    using ParticleGenerator = std::function<void (const ParticleGeneratorContext& context, ParticlePoolComponentView& view)>;
    using ParticleUpdater   = std::function<void (ParticlePoolComponentView& view, float delta_s)>;

    void DefaultGenerator(const ParticleGeneratorContext& context, ParticlePoolComponentView& view);
    void DefaultUpdater(ParticlePoolComponentView& view, float delta_s);

}

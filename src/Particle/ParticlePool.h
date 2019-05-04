
#pragma once

#include "Particle/ParticleFwd.h"
#include "IUpdatable.h"
#include "Math/Vector.h"
#include "Rendering/Color.h"
#include <vector>

namespace mono
{
    class ParticlePool
    {
    public:

        ParticlePool(size_t pool_size, ParticleUpdater update_function);

        void Kill(size_t index);
        void Wake(size_t index);
        void Swap(size_t first, size_t second);

        void doUpdate(const UpdateContext& update_context);

        std::vector<math::Vector> m_position;
        std::vector<math::Vector> m_velocity;

        std::vector<float> m_rotation;
        std::vector<float> m_angular_velocity;

        std::vector<Color::RGBA> m_color;
        std::vector<Color::RGBA> m_start_color;
        std::vector<Color::RGBA> m_end_color;

        std::vector<float> m_size;
        std::vector<float> m_start_size;
        std::vector<float> m_end_size;

        std::vector<int> m_start_life;
        std::vector<int> m_life;

        const size_t m_pool_size = 0;
        size_t m_count_alive = 0;

        const ParticleUpdater m_update_function;
    };
}

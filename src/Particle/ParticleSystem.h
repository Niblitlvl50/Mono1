
#pragma once

#include "ParticleFwd.h"
#include "IGameSystem.h"
#include "Rendering/BlendMode.h"
#include "Rendering/Color.h"
#include "Rendering/RenderFwd.h"
#include "Rendering/Texture/ITextureFactory.h"
#include "Math/Vector.h"
#include "Util/ObjectPool.h"

#include <vector>
#include <memory>

namespace mono
{
    struct ParticleDrawerComponent
    {
        mono::ITexturePtr texture;
        mono::BlendMode blend_mode;
    };

    enum class EmitterType
    {
        CONTINOUS,
        BURST,
        BURST_REMOVE_ON_FINISH
    };

    struct ParticleEmitterComponent
    {
        math::Vector position;
        float duration;
        float elapsed_time;
        float carry_over;

        float emit_rate;
        bool burst_emitted;

        EmitterType type;
        ParticleGenerator generator;
    };

    struct ParticlePoolComponent
    {
        std::vector<math::Vector> position;
        std::vector<math::Vector> velocity;

        std::vector<float> rotation;
        std::vector<float> angular_velocity;

        std::vector<Color::RGBA> color;
        std::vector<Color::RGBA> start_color;
        std::vector<Color::RGBA> end_color;

        std::vector<float> size;
        std::vector<float> start_size;
        std::vector<float> end_size;

        std::vector<int> start_life;
        std::vector<int> life;

        size_t pool_size;
        size_t count_alive;

        ParticleUpdater update_function;
    };

    struct ParticleSystemStats
    {
        uint32_t active_pools;
        uint32_t active_emitters;
    };

    class ParticleSystem : public mono::IGameSystem
    {
    public:

        ParticleSystem(size_t count, size_t n_emitters);
        ~ParticleSystem();

        uint32_t Id() const override;
        const char* Name() const override;
        void Update(const mono::UpdateContext& update_context) override;
        void Sync() override;

        ParticlePoolComponent* AllocatePool(uint32_t id, size_t pool_size, ParticleUpdater update_function);
        void ReleasePool(uint32_t id);
        ParticlePoolComponent* GetPool(uint32_t id);

        void SetPoolDrawData(uint32_t pool_id, mono::ITexturePtr texture, mono::BlendMode blend_mode);

        // duration in seconds, negative value means infinite
        // emit_rate is n particles per second
        // if emitter type is burst, one burst only and will emit duration * emit_rate particles
        ParticleEmitterComponent* AttachEmitter(
            uint32_t pool_id,
            const math::Vector& position,
            float duration_seconds,
            float emit_rate,
            EmitterType emitter_type,
            ParticleGenerator generator);
        
        void ReleaseEmitter(uint32_t pool_id, ParticleEmitterComponent* emitter);
        void SetEmitterPosition(ParticleEmitterComponent* emitter, const math::Vector& position);

        template<typename T>
        void ForEach(T&& callable) const
        {
            for(uint32_t index = 0; index < m_active_pools.size(); ++index)
            {
                const bool is_active = m_active_pools[index];
                if(is_active)
                    callable(index, m_particle_pools[index], m_particle_drawers[index]);
            }
        }

        ParticleSystemStats GetStats() const;

    private:

        void UpdateEmitter(
            ParticleEmitterComponent* emitter, ParticlePoolComponent& particle_pool, uint32_t pool_id, const mono::UpdateContext& update_context);

        std::vector<ParticlePoolComponent> m_particle_pools;
        std::vector<ParticleDrawerComponent> m_particle_drawers;
        std::vector<bool> m_active_pools;

        mono::ObjectPool<ParticleEmitterComponent> m_particle_emitters;
        std::vector<std::vector<ParticleEmitterComponent*>> m_particle_pools_emitters;

        struct DeferredReleasEmitter
        {
            uint32_t pool_id;
            ParticleEmitterComponent* emitter;
        };

        std::vector<DeferredReleasEmitter> m_deferred_release_emitter;
    };
}

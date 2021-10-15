
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

    enum class EmitterType : int
    {
        CONTINOUS,
        BURST,
        BURST_REMOVE_ON_FINISH
    };

    struct ParticleEmitterComponent
    {
        math::Vector position;
        float duration;
        float emit_rate;
        EmitterType type;
        ParticleGenerator generator;

        // Internal data
        float elapsed_time;
        float carry_over;
        bool burst_emitted;
    };

    struct ParticlePoolComponent
    {
        std::vector<math::Vector> position;
        std::vector<math::Vector> velocity;

        std::vector<float> rotation;
        std::vector<float> angular_velocity;

        std::vector<Color::RGBA> color;
        std::vector<Color::Gradient<4>> gradient;

        std::vector<float> size;
        std::vector<float> start_size;
        std::vector<float> end_size;

        std::vector<int> life;
        std::vector<int> start_life;

        uint32_t pool_size;
        uint32_t count_alive;

        ParticleUpdater update_function;
    };

    struct ParticlePoolComponentView
    {
        math::Vector& position;
        math::Vector& velocity;

        float& rotation;
        float& angular_velocity;

        mono::Color::RGBA& color;
        mono::Color::Gradient<4>& gradient;

        float& size;
        float& start_size;
        float& end_size;

        int& life;
        int& start_life;
    };

    struct ParticleSystemStats
    {
        uint32_t active_pools;
        uint32_t active_emitters;
    };

    class ParticleSystem : public mono::IGameSystem
    {
    public:

        ParticleSystem(uint32_t count, uint32_t n_emitters);
        ~ParticleSystem();

        uint32_t Id() const override;
        const char* Name() const override;
        void Update(const mono::UpdateContext& update_context) override;
        void Sync() override;

        ParticlePoolComponent* AllocatePool(uint32_t id);
        ParticlePoolComponent* AllocatePool(uint32_t id, uint32_t pool_size, ParticleUpdater update_function);
        void ReleasePool(uint32_t id);
        void SetPoolData(uint32_t id, uint32_t pool_size, const char* texture_file, mono::BlendMode blend_mode, ParticleUpdater update_function);
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
        void RestartEmitter(ParticleEmitterComponent* emitter);
        const std::vector<ParticleEmitterComponent*>& GetAttachedEmitters(uint32_t pool_id) const;

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

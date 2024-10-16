
#pragma once

#include "IGameSystem.h"
#include "MonoFwd.h"
#include "Math/Quad.h"
#include "Physics/PhysicsFwd.h"
#include "TriggerTypes.h"
#include "Util/ActiveVector.h"

#include <cstdint>
#include <vector>
#include <array>
#include <unordered_map>
#include <functional>
#include <memory>

namespace mono
{
    struct ShapeTriggerComponent
    {
        uint32_t trigger_hash_enter;
        uint32_t trigger_hash_exit;
        uint32_t collision_mask;

        // Internal data
        std::unique_ptr<mono::ICollisionHandler> shape_trigger_handler;
    };

    struct AreaEntityTriggerComponent
    {
        uint32_t trigger_hash;
        math::Quad world_bb;
        uint32_t faction;
        int n_entities;
        AreaTriggerOperation operation;
    };

    struct TimeTriggerComponent
    {
        uint32_t trigger_hash;
        float timeout_ms;
        float timeout_counter_ms;
        bool repeating;
    };

    struct CounterTriggerComponent
    {
        uint32_t listen_trigger_hash;
        uint32_t completed_trigger_hash;
        bool reset_on_completed;
        int count;

        // Internal data
        uint32_t callback_id;
        int counter;
    };

    struct RelayTriggerComponent
    {
        uint32_t listen_trigger_hash;
        uint32_t completed_trigger_hash;
        int delay_ms;

        // Internal data
        uint32_t callback_id;
    };

    // Argument is the hash value of the trigger that was emitted.
    using TriggerCallback = std::function<void (uint32_t trigger_id)>;

    class TriggerSystem : public mono::IGameSystem
    {
    public:

        TriggerSystem(
            uint32_t n_triggers,
            mono::PhysicsSystem* physics_system);

        ShapeTriggerComponent* AllocateShapeTrigger(uint32_t entity_id);
        void ReleaseShapeTrigger(uint32_t entity_id);
        void AddShapeTrigger(
            uint32_t entity_id, uint32_t trigger_hash_enter, uint32_t trigger_hash_exit, uint32_t collision_mask, bool trigger_once);

        AreaEntityTriggerComponent* AllocateAreaTrigger(uint32_t entity_id);
        void ReleaseAreaTrigger(uint32_t entity_id);
        void AddAreaEntityTrigger(
            uint32_t entity_id, uint32_t trigger_hash, const math::Quad& world_bb, uint32_t faction, AreaTriggerOperation operation, int n_entities);

        TimeTriggerComponent* AllocateTimeTrigger(uint32_t entity_id);
        void ReleaseTimeTrigger(uint32_t entity_id);
        void AddTimeTrigger(uint32_t entity_id, uint32_t trigger_hash, float timeout_ms, bool repeating);

        CounterTriggerComponent* AllocateCounterTrigger(uint32_t entity_id);
        void ReleaseCounterTrigger(uint32_t entity_id);
        void AddCounterTrigger(
            uint32_t entity_id, uint32_t listener_hash, uint32_t completed_hash, int count, bool reset_on_completed);

        RelayTriggerComponent* AllocateRelayTrigger(uint32_t entity_id);
        void ReleaseRelayTrigger(uint32_t entity_id);
        void AddRelayTrigger(uint32_t entity_id, uint32_t listener_hash, uint32_t completed_hash, int delay_ms);

        [[nodiscard]]
        uint32_t RegisterTriggerCallback(uint32_t trigger_hash, TriggerCallback callback, uint32_t debug_entity_id);
        void RemoveTriggerCallback(uint32_t trigger_hash, uint32_t callback_id, uint32_t debug_entity_id);

        const std::unordered_map<uint32_t, std::vector<uint32_t>>& GetTriggerTargets() const;

        void EmitTrigger(uint32_t trigger_hash);

        template<typename T>
        void ForEachShapeTrigger(T&& callable)
        {
            m_shape_triggers.ForEach(callable);
        }

        template<typename T>
        void ForEachAreaTrigger(T&& callable)
        {
            m_area_triggers.ForEach(callable);
        }

        template<typename T>
        void ForEachTimeTrigger(T&& callable)
        {
            m_time_triggers.ForEach(callable);
        }

        template<typename T>
        void ForEachCounterTrigger(T&& callable)
        {
            m_counter_triggers.ForEach(callable);
        }

        template<typename T>
        void ForEeachRelayTrigger(T&& callable)
        {
            m_relay_triggers.ForEach(callable);
        }

        const char* Name() const override;
        void Update(const mono::UpdateContext& update_context) override;

    private:

        void UpdateAreaEntityTriggers(const mono::UpdateContext& update_context);
        void UpdateTimeTriggers(const mono::UpdateContext& update_context);
        void UpdateDelayedRelayTriggers(const mono::UpdateContext& update_context);

        mono::PhysicsSystem* m_physics_system;

        using TriggerCallbacks = std::array<TriggerCallback, 8>;
        std::unordered_map<uint32_t, TriggerCallbacks> m_trigger_callbacks;

        mono::ActiveVector<ShapeTriggerComponent> m_shape_triggers;
        mono::ActiveVector<AreaEntityTriggerComponent> m_area_triggers;
        mono::ActiveVector<TimeTriggerComponent> m_time_triggers;
        mono::ActiveVector<CounterTriggerComponent> m_counter_triggers;
        mono::ActiveVector<RelayTriggerComponent> m_relay_triggers;

        float m_area_trigger_timer_s;

        struct DelayRelayTrigger
        {
            uint32_t relay_trigger_hash;
            int delay_ms;
        };
        std::vector<DelayRelayTrigger> m_delay_relay_triggers;
        std::vector<uint32_t> m_triggers_to_emit;

        // Debug data
        std::unordered_map<uint32_t, std::vector<uint32_t>> m_trigger_hash_to_entity_ids;
    };
}

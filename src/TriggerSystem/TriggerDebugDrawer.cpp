
#include "TriggerDebugDrawer.h"
#include "TriggerSystem.h"

#include "Math/MathFunctions.h"
#include "Math/Quad.h"
#include "Rendering/Color.h"
#include "Rendering/IRenderer.h"
#include "TransformSystem/TransformSystem.h"
#include "EntitySystem/Entity.h"

using namespace mono;

namespace
{
    std::vector<math::Vector> GenerateArrow(const math::Vector& start, const math::Vector& end)
    {
        const float angle = math::AngleBetweenPoints(start, end);

        const float x1 = std::sin(angle + math::PI() + math::PI_4()) + end.x;
        const float y1 = -std::cos(angle + math::PI() + math::PI_4()) + end.y;

        const float x2 = std::sin(angle - math::PI_4()) + end.x;
        const float y2 = -std::cos(angle - math::PI_4()) + end.y;

        return {
            math::Vector(x1, y1),
            end,
            end,
            math::Vector(x2, y2)
        };
    }
}

TriggerDebugDrawer::TriggerDebugDrawer(const bool& enabled, TriggerSystem* trigger_system, const mono::TransformSystem* transform_system)
    : m_enabled(enabled)
    , m_trigger_system(trigger_system)
    , m_transform_system(transform_system)
{ }

void TriggerDebugDrawer::Draw(mono::IRenderer& renderer) const
{
    if(!m_enabled)
        return;

    std::unordered_map<uint32_t, std::vector<uint32_t>> trigger_hash_to_emitter_ids;

    const auto collect_shape_triggers = [&](uint32_t entity_id, const ShapeTriggerComponent& trigger)
    {
        trigger_hash_to_emitter_ids[trigger.trigger_hash_enter].push_back(entity_id);
        trigger_hash_to_emitter_ids[trigger.trigger_hash_exit].push_back(entity_id);
    };

/*
    const auto collect_death_triggers = [&](uint32_t entity_id, const DestroyedTriggerComponent& trigger)
    {
        trigger_hash_to_emitter_ids[trigger.trigger_hash].push_back(entity_id);
    };
*/

    const auto collect_area_triggers = [&](uint32_t entity_id, const AreaEntityTriggerComponent& trigger)
    {
        trigger_hash_to_emitter_ids[trigger.trigger_hash].push_back(entity_id);
    };

    const auto collect_time_triggers = [&](uint32_t entity_id, const TimeTriggerComponent& trigger)
    {
        trigger_hash_to_emitter_ids[trigger.trigger_hash].push_back(entity_id);
    };

    const auto collect_counter_triggers = [&](uint32_t entity_id, const CounterTriggerComponent& trigger)
    {
        trigger_hash_to_emitter_ids[trigger.completed_trigger_hash].push_back(entity_id);
    };

    const auto collect_relay_triggers = [&](uint32_t entity_id, const RelayTriggerComponent& trigger)
    {
        trigger_hash_to_emitter_ids[trigger.completed_trigger_hash].push_back(entity_id);
    };

    m_trigger_system->ForEachShapeTrigger(collect_shape_triggers);
    //m_trigger_system->ForEachDestroyedTrigger(collect_death_triggers);
    m_trigger_system->ForEachAreaTrigger(collect_area_triggers);
    m_trigger_system->ForEachTimeTrigger(collect_time_triggers);
    m_trigger_system->ForEachCounterTrigger(collect_counter_triggers);
    m_trigger_system->ForEeachRelayTrigger(collect_relay_triggers);

    const std::unordered_map<uint32_t, std::vector<uint32_t>>& targets = m_trigger_system->GetTriggerTargets();

    for(const auto& hash_emitters : trigger_hash_to_emitter_ids)
    {
        std::vector<math::Vector> emitter_positions_for_this_hash;
        std::vector<math::Vector> target_positions_for_this_hash;

        for(uint32_t emitter_entity_id : hash_emitters.second)
        {
            if(emitter_entity_id == mono::INVALID_ID)
                continue;

            const math::Matrix& world_transform = m_transform_system->GetWorld(emitter_entity_id);
            const math::Vector& world_position = math::GetPosition(world_transform);
            emitter_positions_for_this_hash.push_back(world_position);
        }

        const auto it = targets.find(hash_emitters.first);
        if(it != targets.end())
        {
            for(uint32_t target_entity_id : it->second)
            {
                if(target_entity_id == mono::INVALID_ID) // This can mean the target resides in code
                    continue;

                const math::Matrix& world_transform = m_transform_system->GetWorld(target_entity_id);
                const math::Vector& world_position = math::GetPosition(world_transform);
                target_positions_for_this_hash.push_back(world_position);
            }
        }

        if(!target_positions_for_this_hash.empty())
        {
            std::vector<math::Vector> lines;
            lines.reserve(emitter_positions_for_this_hash.size() * target_positions_for_this_hash.size());

            for(const math::Vector& emitter_position : emitter_positions_for_this_hash)
            {
                for(const math::Vector& target_position : target_positions_for_this_hash)
                {
                    lines.push_back(emitter_position);
                    lines.push_back(target_position);
                    
                    const std::vector<math::Vector> arrow = GenerateArrow(emitter_position, target_position);
                    lines.insert(lines.end(), arrow.begin(), arrow.end());
                }
            }

            renderer.DrawLines(lines, mono::Color::CYAN, 1.0f);
        }
    }
}

math::Quad TriggerDebugDrawer::BoundingBox() const
{
    return math::InfQuad;
}

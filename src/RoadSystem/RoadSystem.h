
#pragma once

#include "IGameSystem.h"
#include <vector>
#include <string>

namespace mono
{
    struct RoadComponent
    {
        float width;
        std::string texture_name;
    };

    class RoadSystem : public mono::IGameSystem
    {
    public:

        RoadSystem(uint32_t n);

        RoadComponent* Allocate(uint32_t entity_id);
        void Release(uint32_t entity_id);
        void SetData(uint32_t entity_id, const RoadComponent& component);

        uint32_t Id() const override;
        const char* Name() const override;
        void Update(const mono::UpdateContext& update_context) override;

        template <typename T>
        inline void ForEeach(T&& func) const
        {
            for(size_t entity_id = 0; entity_id < m_active.size(); ++entity_id)
            {
                if(m_active[entity_id])
                    func(entity_id, m_roads[entity_id]);
            }
        }

        std::vector<bool> m_active;
        std::vector<RoadComponent> m_roads;
    };
}

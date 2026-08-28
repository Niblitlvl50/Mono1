
#pragma once

#include "IGameSystem.h"
#include "Rendering/Color.h"

#include <vector>
#include <string>
#include <cstdint>

namespace mono
{
    struct RiverComponent
    {
        float width;
        mono::Color::RGBA color;
        std::string texture_name;
    };

    class RiverSystem : public mono::IGameSystem
    {
    public:

        RiverSystem(uint32_t n);

        RiverComponent* Allocate(uint32_t entity_id);
        void Release(uint32_t entity_id);
        void SetData(uint32_t entity_id, const RiverComponent& component);

        const char* Name() const override;
        void Update(const mono::UpdateContext& update_context) override;

        template <typename T>
        inline void ForEeach(T&& func) const
        {
            for(uint32_t entity_id = 0; entity_id < m_active.size(); ++entity_id)
            {
                if(m_active[entity_id])
                    func(entity_id, m_rivers[entity_id]);
            }
        }

        std::vector<bool> m_active;
        std::vector<RiverComponent> m_rivers;
    };
}

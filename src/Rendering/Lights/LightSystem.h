
#pragma once

#include "IGameSystem.h"
#include "Rendering/Color.h"
#include <vector>

namespace mono
{
    struct LightComponent
    {
        float radius;
        mono::Color::RGBA shade;
        bool flicker;
        float flicker_frequencey;
        float flicker_percentage;
    };

    class LightSystem : public mono::IGameSystem
    {
    public:

        LightSystem(uint32_t n_lights);

        void Allocate(uint32_t light_id);
        bool IsAllocated(uint32_t light_id);
        void SetData(uint32_t light_id, const LightComponent& component_data);
        void Release(uint32_t light_id);

        uint32_t Id() const override;
        const char* Name() const override;
        void Update(const mono::UpdateContext& update_context) override;

        template <typename T>
        inline void ForEach(T&& callable) const
        {
            for(uint32_t index = 0; index < m_alive.size(); ++index)
            {
                if(m_alive[index])
                    callable(m_lights[index], index);
            }
        }

        std::vector<bool> m_alive;
        std::vector<LightComponent> m_lights;
    };
}

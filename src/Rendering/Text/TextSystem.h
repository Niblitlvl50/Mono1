
#pragma once

#include "IGameSystem.h"
#include "TextFlags.h"
#include "MonoFwd.h"
#include "Rendering/Color.h"

#include <vector>
#include <string>

namespace mono
{
    struct TextComponent
    {
        std::string text;
        int font_id;
        mono::Color::RGBA tint;
        mono::FontCentering center_flags;
        bool draw_shadow;
        math::Vector shadow_offset;
        mono::Color::RGBA shadow_color;
    };

    class TextSystem : public mono::IGameSystem
    {
    public:

        TextSystem(uint32_t n, mono::TransformSystem* transform_system);

        TextComponent* AllocateText(uint32_t id);
        void ReleaseText(uint32_t id);
        void SetTextData(uint32_t id, const TextComponent& text_data);
        void SetTextColor(uint32_t id, const mono::Color::RGBA& new_color);

        const char* Name() const override;
        void Update(const mono::UpdateContext& update_context) override;
        bool UpdateInPause() const override;

        template <typename T>
        inline void ForEach(T&& functor)
        {
            for(size_t index = 0; index < m_texts.size(); ++index)
            {
                const bool is_alive = m_alive[index];
                if(is_alive)
                    functor(m_texts[index], index);
            }
        }

    private:

        mono::TransformSystem* m_transform_system;

        std::vector<TextComponent> m_texts;
        std::vector<bool> m_alive;
        std::vector<bool> m_text_dirty;
    };
}

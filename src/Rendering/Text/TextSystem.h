
#pragma once

#include "IGameSystem.h"
#include "MonoFwd.h"
#include "Rendering/Color.h"

#include <vector>
#include <string>

namespace mono
{
    struct TextComponent
    {
        int font_id;
        std::string text;
        bool centered;
        mono::Color::RGBA tint;
        bool draw_shadow;
    };

    class TextSystem : public mono::IGameSystem
    {
    public:

        TextSystem(uint32_t n, mono::TransformSystem* transform_system);

        TextComponent* AllocateText(uint32_t id);
        void ReleaseText(uint32_t id);
        void SetTextData(uint32_t id, const TextComponent& text_data);

        uint32_t Id() const override;
        const char* Name() const override;
        void Update(const mono::UpdateContext& update_context) override;

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

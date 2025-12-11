
#pragma once

#include "MonoFwd.h"
#include "Rendering/IDrawable.h"
#include "Rendering/RenderFwd.h"
#include "Rendering/Text/TextSystem.h"

#include <vector>
#include <unordered_map>

namespace mono
{
    struct TextDrawBuffers;

    class TextBatchDrawer : public mono::IDrawable
    {
    public:

        TextBatchDrawer(mono::TextSystem* text_system, mono::TransformSystem* transform_system);
        ~TextBatchDrawer();

        void Draw(mono::IRenderer& renderer) const override;
        math::Quad BoundingBox() const override;

    private:

        const TextDrawBuffers* UpdateDrawBuffers(const mono::TextComponent& text_component, uint32_t index) const;

        mono::TextSystem* m_text_system;
        mono::TransformSystem* m_transform_system;

        mutable std::vector<TextDrawBuffers> m_draw_buffers;
        mutable std::unordered_map<uint32_t, mono::TextComponent> m_current_data;
        mutable std::vector<uint32_t> m_last_active_entities;
    };
}

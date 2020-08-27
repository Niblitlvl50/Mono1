
#pragma once

#include "Rendering/IDrawable.h"
#include "MonoFwd.h"

namespace mono
{
    class TextBatchDrawer : public mono::IDrawable
    {
    public:

        TextBatchDrawer(mono::TextSystem* text_system, mono::TransformSystem* transform_system);

        void Draw(mono::IRenderer& renderer) const override;
        math::Quad BoundingBox() const override;

    private:

        mono::TextSystem* m_text_system;
        mono::TransformSystem* m_transform_system;
    };
}

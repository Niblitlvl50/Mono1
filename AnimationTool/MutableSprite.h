
#pragma once

#include "Entity/EntityBase.h"
#include "Rendering/RenderPtrFwd.h"

namespace animator
{
    class MutableSprite : public mono::EntityBase
    {
    public:

        MutableSprite(mono::ISprite& sprite);

        virtual void Draw(mono::IRenderer& renderer) const;
        virtual void Update(unsigned int delta);

    private:

        mono::ISprite& m_sprite;
    };
}


#pragma once

#include "EntityBase.h"
#include "MonoPtrFwd.h"

namespace animator
{
    class MutableSprite : public mono::EntityBase
    {
    public:

        MutableSprite(const mono::ISpritePtr& sprite);

        virtual void Draw(mono::IRenderer& renderer) const;
        virtual void Update(unsigned int delta);

    private:

        mono::ISpritePtr m_sprite;
    };
}

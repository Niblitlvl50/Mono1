
#pragma once

#include "EntityBase.h"
#include "MonoPtrFwd.h"

namespace animator
{
    class MutableSprite : public mono::EntityBase
    {
    public:

        MutableSprite(const char* file);

        virtual void Draw(mono::IRenderer& renderer) const;
        virtual void Update(unsigned int delta);

        void SetAnimation(unsigned int id);
        void NextAnimation();
        void PreviousAnimation();

    private:
    
        mono::ISpritePtr m_sprite;
    };
}


#pragma once

#include "Entity/PhysicsEntityBase.h"
#include "Rendering/RenderPtrFwd.h"

namespace game
{
    class Meteor : public mono::PhysicsEntityBase
    {
    public:
        Meteor(float x, float y);
    
    private:
        virtual void Draw(mono::IRenderer& renderer) const;
        virtual void Update(unsigned int delta);
        
        mono::ISpritePtr mSprite;
    };
}


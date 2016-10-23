
#pragma once

#include "EntityBase.h"
#include "MonoPtrFwd.h"
#include "Sprite/ISprite.h"

namespace animator
{
    class MutableSprite : public mono::EntityBase
    {
    public:

        MutableSprite(const char* file);

        virtual void Draw(mono::IRenderer& renderer) const;
        virtual void Update(unsigned int delta);

        void SetAnimation(int id);
        void RestartAnimation();

        int NextAnimation() const;
        int PreviousAnimation() const;
        int CurrentAnimation() const;

        const mono::AnimationSequence& GetSequence(int id) const;
        mono::AnimationSequence& GetSequence(int id);

        const std::vector<mono::AnimationSequence>& GetAnimations() const;

    private:
    
        mono::ISpritePtr m_sprite;
    };
}


#pragma once

#include "EntityBase.h"

namespace editor
{
    class SpriteEntity : public mono::EntityBase
    {
    public:
        SpriteEntity(const char* file);
        virtual void Draw(mono::IRenderer& renderer) const;
        virtual void Update(unsigned int delta);

        void SetSelected(bool selected);
    
    private:

        bool m_selected;
        mono::ISpritePtr m_sprite;
    };
}

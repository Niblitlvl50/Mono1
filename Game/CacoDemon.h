//
//  CacoDemon.hpp
//  MonoiOS
//
//  Created by Niklas Damberg on 22/11/15.
//
//

#pragma once

#include "PhysicsEntityBase.h"
#include "Sprite.h"
#include "CacoDemonController.h"
#include "IWeaponSystem.h"

namespace game
{
    class CacoDemon : public mono::PhysicsEntityBase
    {
    public:

        CacoDemon(mono::EventHandler& eventHandler);

        virtual void Draw(mono::IRenderer& renderer) const;
        virtual void Update(unsigned int delta);

    private:

        mono::EventHandler& m_eventHandler;
        mono::Sprite m_sprite;
        CacoDemonController m_controller;
        std::unique_ptr<IWeaponSystem> m_weapon;
    };
}
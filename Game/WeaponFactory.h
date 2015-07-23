
//
//  Header.h
//  MonoOSX
//
//  Created by Niklas Damberg on 19/07/15.
//
//

#pragma once

#include "WeaponTypes.h"
#include "MonoFwd.h"

#include <memory>

namespace game
{
    class IWeaponSystem;
}

namespace Factory
{
    std::unique_ptr<game::IWeaponSystem> CreateWeapon(game::WeaponType weapon, mono::EventHandler& eventHandler);
}

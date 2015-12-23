//
//  WeaponFactory.cpp
//  MonoOSX
//
//  Created by Niklas Damberg on 19/07/15.
//
//

#include "WeaponFactory.h"
#include "StandardWeapon.h"
#include "RocketLauncher.h"
#include "CacoPlasma.h"

std::unique_ptr<game::IWeaponSystem> game::Factory::CreateWeapon(game::WeaponType weapon, mono::EventHandler& eventHandler)
{
    switch(weapon)
    {
        case game::WeaponType::STANDARD:
            return std::unique_ptr<game::StandardWeapon>(new game::StandardWeapon(eventHandler));
        case game::WeaponType::ROCKET:
            return std::unique_ptr<game::RocketLauncher>(new game::RocketLauncher(eventHandler));
        case game::WeaponType::CACOPLASMA:
            return std::unique_ptr<game::CacoPlasma>(new game::CacoPlasma(eventHandler));
    }
}

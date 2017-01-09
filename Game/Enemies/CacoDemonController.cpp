
#include "CacoDemonController.h"
#include "Weapons/IWeaponSystem.h"
#include "Weapons/IWeaponFactory.h"
#include "Factories.h"
#include "Sprite/ISprite.h"
#include <cmath>

using namespace game;

namespace
{
    enum Animation
    {
        IDLE,
        ATTACK,
        HURT,
        DEATH
    };
}

CacoDemonController::CacoDemonController(mono::EventHandler& event_handler)
{
    m_weapon = weapon_factory->CreateWeapon(WeaponType::CACOPLASMA, WeaponFaction::ENEMY);
}

void CacoDemonController::Initialize(Enemy* enemy)
{
    m_enemy = enemy;
    m_enemy->m_sprite->SetAnimation(Animation::ATTACK);
}

void CacoDemonController::doUpdate(unsigned int delta)
{
    m_weapon->Fire(m_enemy->Position(), m_enemy->Rotation());
}


#include "CacoDemonController.h"
#include "Weapons/IWeaponSystem.h"
#include "Weapons/WeaponFactory.h"
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
    m_weapon = Factory::CreateWeapon(WeaponType::CACOPLASMA, WeaponFaction::ENEMY, event_handler);
}

void CacoDemonController::Initialize(Enemy* enemy)
{
    m_enemy = enemy;
    m_enemy->m_sprite->SetAnimation(Animation::ATTACK);
}

void CacoDemonController::doUpdate(unsigned int delta)
{
    const float rotation = m_enemy->Rotation();

    const math::Vector unit(-std::sin(rotation), std::cos(rotation));
    const math::Vector offset = unit * 30.0f;

    m_weapon->Fire(m_enemy->Position() + offset, rotation);
}

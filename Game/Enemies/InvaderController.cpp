
#include "InvaderController.h"
#include "EntityProperties.h"
#include "Sprite/ISprite.h"

using namespace game;

InvaderController::InvaderController(mono::EventHandler& event_handler)
{
    (void)event_handler;
}

void InvaderController::Initialize(Enemy* enemy)
{
    m_enemy = enemy;
    m_enemy->SetProperty(EntityProperties::DAMAGABLE);
}

void InvaderController::doUpdate(unsigned int delta)
{
    m_color.hue += (delta * 0.0005f);
    if(m_color.hue > 1.0f)
        m_color.hue = 0.0f;

    m_enemy->m_sprite->SetShade(mono::Color::ToRGBA(m_color));
}

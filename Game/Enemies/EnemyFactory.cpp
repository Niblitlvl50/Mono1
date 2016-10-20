
#include "EnemyFactory.h"
#include "Enemy.h"

#include "CacoDemonController.h"
#include "RyuController.h"

std::shared_ptr<game::Enemy> game::CreateCacoDemon(const math::Vector2f& position, mono::EventHandler& event_handler)
{
    EnemySetup setup;
    setup.sprite_file = "sprites/cacodemon.sprite";
    setup.size = 50.0f;
    setup.mass = 500.0f;
    setup.position = position;
    setup.controller = std::make_unique<CacoDemonController>(event_handler);

    return std::make_shared<game::Enemy>(setup);
}

std::shared_ptr<game::Enemy> game::CreateRyu(const math::Vector2f& position, mono::EventHandler& event_handler)
{
    EnemySetup setup;
    setup.sprite_file = "sprites/ryu.sprite";
    setup.size = 30.0f;
    setup.mass = 80.0f;
    setup.position = position;
    setup.controller = std::make_unique<RyuController>(event_handler);

    return std::make_shared<game::Enemy>(setup);
}

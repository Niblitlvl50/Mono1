
#include "EnemyFactory.h"
#include "Enemy.h"
#include "Paths/IPath.h"

#include "CacoDemonController.h"
#include "RyuController.h"
#include "InvaderController.h"

std::shared_ptr<game::Enemy> game::CreateCacoDemon(const math::Vector& position, mono::EventHandler& event_handler)
{
    EnemySetup setup;
    setup.sprite_file = "sprites/cacodemon.sprite";
    setup.size = 50.0f;
    setup.mass = 500.0f;
    setup.position = position;
    setup.controller = std::make_unique<CacoDemonController>(event_handler);

    return std::make_shared<game::Enemy>(setup);
}

std::shared_ptr<game::Enemy> game::CreateRyu(const math::Vector& position, mono::EventHandler& event_handler)
{
    EnemySetup setup;
    setup.sprite_file = "sprites/ryu.sprite";
    setup.size = 30.0f;
    setup.mass = 80.0f;
    setup.position = position;
    setup.controller = std::make_unique<RyuController>(event_handler);

    return std::make_shared<game::Enemy>(setup);
}

std::shared_ptr<game::Enemy> game::CreateInvader(const math::Vector& position, mono::EventHandler& event_handler)
{
    EnemySetup setup;
    setup.sprite_file = "sprites/invader.sprite";
    setup.size = 20.0f;
    setup.mass = 50.0f;
    setup.position = position;
    setup.controller = std::make_unique<InvaderController>(event_handler);

    return std::make_shared<game::Enemy>(setup);
}

game::EnemyPtr game::CreatePathInvader(const mono::IPathPtr& path, mono::EventHandler& event_handler)
{
    EnemySetup setup;
    setup.sprite_file = "sprites/invader.sprite";
    setup.size = 20.0f;
    setup.mass = 10.0f;
    setup.position = path->GetGlobalPosition();
    setup.controller = std::make_unique<InvaderPathController>(path, event_handler);

    return std::make_shared<game::Enemy>(setup);
}


#include "EnemyFactory.h"
#include "Enemy.h"
#include "Paths/IPath.h"

#include "CacoDemonController.h"
#include "RyuController.h"
#include "InvaderController.h"
#include "BlackSquareController.h"


using namespace game;

EnemyFactory::EnemyFactory(mono::EventHandler& event_handler)
    : m_eventHandler(event_handler)
{ }

game::EnemyPtr EnemyFactory::CreateCacoDemon(const math::Vector& position)
{
    EnemySetup setup;
    setup.sprite_file = "sprites/cacodemon.sprite";
    setup.size = 50.0f;
    setup.mass = 500.0f;
    setup.position = position;
    setup.controller = std::make_unique<CacoDemonController>(m_eventHandler);

    return std::make_shared<game::Enemy>(setup);
}

game::EnemyPtr EnemyFactory::CreateRyu(const math::Vector& position)
{
    EnemySetup setup;
    setup.sprite_file = "sprites/ryu.sprite";
    setup.size = 30.0f;
    setup.mass = 80.0f;
    setup.position = position;
    setup.controller = std::make_unique<RyuController>(m_eventHandler);

    return std::make_shared<game::Enemy>(setup);
}

game::EnemyPtr EnemyFactory::CreateInvader(const math::Vector& position)
{
    EnemySetup setup;
    setup.sprite_file = "sprites/invader.sprite";
    setup.size = 20.0f;
    setup.mass = 50.0f;
    setup.position = position;
    setup.controller = std::make_unique<InvaderController>(m_eventHandler);

    return std::make_shared<game::Enemy>(setup);
}

game::EnemyPtr EnemyFactory::CreatePathInvader(const mono::IPathPtr& path)
{
    EnemySetup setup;
    setup.sprite_file = "sprites/invader.sprite";
    setup.size = 20.0f;
    setup.mass = 50.0f;
    setup.position = path->GetGlobalPosition();
    setup.controller = std::make_unique<InvaderPathController>(path, m_eventHandler);

    return std::make_shared<game::Enemy>(setup);
}

game::EnemyPtr EnemyFactory::CreateBlackSquare(const math::Vector& position)
{
    EnemySetup setup;
    setup.sprite_file = "sprites/invader.sprite";
    setup.size = 20.0f;
    setup.mass = 50.0f;
    setup.position = position;
    setup.controller = std::make_unique<BlackSquareController>(200.0f, m_eventHandler);

    return std::make_shared<game::Enemy>(setup);
}


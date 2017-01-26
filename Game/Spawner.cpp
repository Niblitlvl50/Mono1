
#include "Spawner.h"
#include "EventHandler/EventHandler.h"
#include "Events/SpawnPhysicsEntityEvent.h"
#include "Meteor.h"
#include "Random.h"
#include "RenderLayers.h"

using namespace game;

Spawner::Spawner(mono::EventHandler& eventHandler)
{
    const auto spawn_func = [&eventHandler]() {
        const float x = mono::Random(-50.0f, 50.0f);
        const float y = mono::Random(-50.0f, 50.0f);

        const game::SpawnPhysicsEntityEvent event(std::make_shared<Meteor>(x, y), BACKGROUND);
        eventHandler.DispatchEvent(event);
    };

    m_timer = Time::CreateRepeatingTimer(1000, spawn_func);
}

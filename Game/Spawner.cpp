
#include "Spawner.h"
#include "EventHandler/EventHandler.h"
#include "Events/SpawnPhysicsEntityEvent.h"
#include "Meteor.h"
#include "Random.h"
#include "RenderLayers.h"

using namespace game;

Spawner::Spawner(mono::EventHandler& eventHandler)
{
    const auto spawn_func = [](void* data) {
        mono::EventHandler* event_handler = static_cast<mono::EventHandler*>(data);

        const float x = mono::Random(-50.0f, 50.0f);
        const float y = mono::Random(-50.0f, 50.0f);

        const game::SpawnPhysicsEntityEvent event(std::make_shared<Meteor>(x, y), BACKGROUND);
        event_handler->DispatchEvent(event);
    };

    m_timer.reset(System2::CreateRepeatingTimer(1000, spawn_func, &eventHandler));
}

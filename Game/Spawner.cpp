//
//  Spawner.cpp
//  MonoOSX
//
//  Created by Niklas Damberg on 03/10/15.
//
//

#include "Spawner.h"
#include "EventHandler.h"
#include "SpawnPhysicsEntityEvent.h"
#include "Meteor.h"
#include "Utils.h"

using namespace game;

Spawner::Spawner(mono::EventHandler& eventHandler)
    : m_eventHandler(eventHandler)
{
    m_timer = Time::CreateRepeatingTimer(1000, std::bind(&Spawner::OnSpawnEntity, this));
}

void Spawner::OnSpawnEntity()
{
    const float x = mono::Random() * 1000 - 500;
    const float y = mono::Random() * 1000 - 500;
    
    game::SpawnPhysicsEntityEvent event(std::make_shared<Meteor>(x, y));
    m_eventHandler.DispatchEvent(event);
}

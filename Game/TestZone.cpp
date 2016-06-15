//
//  TestZone.cpp
//  Mono1
//
//  Created by Niblit on 2012-04-06.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "TestZone.h"
#include "ICamera.h"
#include "AudioFactory.h"

#include "PhysicsGrid.h"
#include "InvaderGroup.h"
#include "DotEntity.h"
#include "AnimatedDude.h"
#include "Shuttle.h"
#include "Moon.h"
#include "Meteor.h"
#include "PathPoint.h"
#include "CubeSwarm.h"
#include "CacoDemon.h"
#include "Morpher.h"

#include "Quad.h"

#include "EventHandler.h"
#include "SpawnEntityEvent.h"
#include "SpawnPhysicsEntityEvent.h"
#include "RemoveEntityEvent.h"
#include "ShockwaveEvent.h"
#include "DamageEvent.h"

#include "MathFunctions.h"
#include "Utils.h"
#include "EntityProperties.h"
#include "RenderLayers.h"
#include "AudioListener.h"

#include <cmath>
#include <thread>


using namespace game;
using namespace std::placeholders;


namespace
{
    struct GravityUpdater : mono::IUpdatable
    {
        GravityUpdater(mono::IPhysicsZone* zone, mono::IEntityPtr moon1, mono::IEntityPtr moon2)
            : mZone(zone),
              mMoon1(moon1),
              mMoon2(moon2),
              mElapsedTime(0)
        { }
        virtual void doUpdate(unsigned int delta)
        {
            mElapsedTime += delta;
            if(mElapsedTime < 16)
                return;
            
            mZone->ForEachBody(std::bind(&GravityUpdater::GravityFunc, this, _1));
            mElapsedTime = 0;
        }
        void GravityFunc(const mono::IBodyPtr& body)
        {
            math::Vector2f impulse;
            
            math::Vector2f newPos = body->GetPosition() - mMoon1->Position();
            const float distance = math::Length(newPos);
            if(distance < 300.0f)
            {
                const float gravity = 1e4f;
                const float value = -gravity / (distance * std::sqrtf(distance));
            
                math::Normalize(newPos);
                newPos *= value;
                
                impulse += newPos;
            }
            
            math::Vector2f newPos2 = body->GetPosition() - mMoon2->Position();
            const float distance2 = math::Length(newPos2);
            if(distance2 < 200.0f)
            {
                const float gravity = 1e4f;
                const float value = -gravity / (distance2 * std::sqrtf(distance2));
                
                math::Normalize(newPos2);
                newPos2 *= value;
                
                impulse += newPos2;
            }
            
            body->ApplyImpulse(impulse, body->GetPosition());
        }
        
        mono::IPhysicsZone* mZone;
        mono::IEntityPtr mMoon1;
        mono::IEntityPtr mMoon2;
        unsigned int mElapsedTime;
    };

    class ListenerPositionUpdater : public mono::IUpdatable
    {
    public:

        ListenerPositionUpdater(const mono::ICameraPtr& camera)
            : m_camera(camera)
        { }

        virtual void doUpdate(unsigned int delta)
        {
            mono::ListenerPosition(m_camera->GetPosition());
        }

        mono::ICameraPtr m_camera;
    };

    void ApplyShockwave(const mono::IBodyPtr& body, const math::Vector2f& position, float magnitude)
    {
        math::Vector2f unit = body->GetPosition() - position;
        const float length = math::Length(unit);
        if(length > 50)
            return;

        math::Normalize(unit);
        
        const math::Vector2f& impulse = unit * magnitude;
        body->ApplyImpulse(impulse, math::zeroVec);
    }

    void AddMeteorCluster(mono::IPhysicsZone* zone)
    {
        const float x = -400.0f;
        const float y = 400.0f;

        for(int index = 0; index < 10; ++index)
        {
            const float radius = mono::Random() * 50.0f + 80.0f;

            const float sinex = std::sin(mono::Random() * math::PI() * 2.0f);
            const float cosiney = std::cos(mono::Random() * math::PI() * 2.0f);

            const float posx = sinex * radius + x;
            const float posy = cosiney * radius + y;

            auto entity = std::make_shared<Meteor>(posx, posy);
            zone->AddPhysicsEntity(entity, FOREGROUND);
        }
    }

}

TestZone::TestZone(mono::EventHandler& eventHandler)
    : PhysicsZone(math::Vector2f(0.0f, 0.0f), 0.9f),
      mEventHandler(eventHandler),
      m_spawner(eventHandler)
{
    const game::SpawnEntityFunc spawnEntityFunc = std::bind(&TestZone::SpawnEntity, this, _1);
    mSpawnEntityToken = mEventHandler.AddListener(spawnEntityFunc);
    
    const game::SpawnPhysicsEntityFunc spawnPhysicsFunc = std::bind(&TestZone::SpawnPhysicsEntity, this, _1);
    mSpawnPhysicsEntityToken = mEventHandler.AddListener(spawnPhysicsFunc);

    const game::RemoveEntityFunc& removeFunc = std::bind(&TestZone::OnRemoveEntity, this, _1);
    mRemoveEntityByIdToken = mEventHandler.AddListener(removeFunc);

    const game::ShockwaveEventFunc shockwaveFunc = std::bind(&TestZone::OnShockwaveEvent, this, _1);
    mShockwaveEventToken = mEventHandler.AddListener(shockwaveFunc);

    const std::function<void (const game::DamageEvent&)>& damageFunc = std::bind(&TestZone::OnDamageEvent, this, _1);
    mDamageEventToken = mEventHandler.AddListener(damageFunc);

    m_backgroundMusic = mono::AudioFactory::CreateSound("InGame_Phoenix.wav", true);

    m_socket = Network::OpenUDPSocket("127.0.0.1", 5001, false);
    m_outSocket = Network::OpenUDPSocket("", 7777, true);

    /*
    auto func = [this]() {

        std::vector<byte> buffer;
        buffer.resize(300);

        while(true)
        {
            m_outSocket->Receive(buffer);
            if(!buffer.empty())
            {
                std::printf("%s\n", buffer.data());
                std::fill(buffer.begin(), buffer.begin() + buffer.size(), '\0');
            }
        }
    };

    new std::thread(func);
     */
}

TestZone::~TestZone()
{
    mEventHandler.RemoveListener(mSpawnEntityToken);
    mEventHandler.RemoveListener(mSpawnPhysicsEntityToken);
    mEventHandler.RemoveListener(mRemoveEntityByIdToken);
    mEventHandler.RemoveListener(mDamageEventToken);
}

void TestZone::OnLoad(mono::ICameraPtr camera)
{
    std::shared_ptr<PhysicsGrid> bounds = std::make_shared<PhysicsGrid>(math::Quad(-1000.0f, -1000.0f, 1000.0f, 1000.0f));
    AddPhysicsEntity(bounds, BACKGROUND);

    std::shared_ptr<Moon> moon1 = std::make_shared<Moon>(550.0f, 300.0f, 100.0f);
    std::shared_ptr<Moon> moon2 = std::make_shared<Moon>(-400.0f, 400.0f, 50.0f);
    AddPhysicsEntity(moon1, FOREGROUND);
    AddPhysicsEntity(moon2, FOREGROUND);
    AddUpdatable(std::make_shared<GravityUpdater>(this, moon1, moon2));

    std::shared_ptr<Shuttle> shuttle = std::make_shared<Shuttle>(-100.0f, 0.0f, mEventHandler);
    AddPhysicsEntity(shuttle, FOREGROUND);

    AddPhysicsEntity(std::make_shared<game::CacoDemon>(mEventHandler), FOREGROUND);

    AddEntity(std::make_shared<AnimatedDude>(100.0f, 50.0f, mEventHandler), MIDDLEGROUND);
    AddEntity(std::make_shared<InvaderGroup>(), BACKGROUND);
    AddEntity(std::make_shared<DotEntity>(), FOREGROUND);
    AddEntity(std::make_shared<PathPoint>(), BACKGROUND);
    AddEntity(std::make_shared<Morpher>(), FOREGROUND);
    AddEntity(std::make_shared<game::CubeSwarm>(), FOREGROUND);

    AddMeteorCluster(this);

    camera->SetPosition(shuttle->Position());
    camera->Follow(shuttle, math::Vector2f(0, -100));

    m_backgroundMusic->Play();
}

void TestZone::OnUnload()
{ }

void TestZone::SpawnEntity(const game::SpawnEntityEvent& event)
{
    AddEntity(event.mEntity, FOREGROUND);
}

void TestZone::SpawnPhysicsEntity(const game::SpawnPhysicsEntityEvent& event)
{
    AddPhysicsEntity(event.mEntity, FOREGROUND);
}

void TestZone::OnRemoveEntity(const game::RemoveEntityEvent& event)
{
    mono::IPhysicsEntityPtr physics_entity = FindPhysicsEntityFromId(event.id);
    if(physics_entity)
    {
        SchedulePreFrameTask(std::bind(&TestZone::RemovePhysicsEntity, this, physics_entity));
        return;
    }

    mono::IEntityPtr entity = FindEntityFromId(event.id);
    if(entity)
        SchedulePreFrameTask(std::bind(&TestZone::RemoveEntity, this, entity));
}

void TestZone::OnShockwaveEvent(const game::ShockwaveEvent& event)
{
    const auto func = std::bind(ApplyShockwave, _1, event.mPosition, event.mMagnitude);
    this->ForEachBody(func);


    const std::vector<byte> data = { 'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!' };
    m_socket->Send(data);
}

void TestZone::OnDamageEvent(const game::DamageEvent& event)
{
    mono::IPhysicsEntityPtr entity = FindPhysicsEntityFromBody(event.body);
    if(!entity)
        return;

    const DamageResult& result = m_damageController.ApplyDamage(entity->Id(), event.damage);
    if(!result.success)
        return;

    if(result.health_left <= 0)
    {
        m_damageController.RemoveRecord(entity->Id());
        SchedulePreFrameTask(std::bind(&TestZone::RemovePhysicsEntity, this, entity));
    }
}

void TestZone::AddPhysicsEntity(const mono::IPhysicsEntityPtr& entity, int layer)
{
    const bool damagable = entity->HasProperty(EntityProperties::DAMAGABLE);
    if(damagable)
        m_damageController.CreateRecord(entity->Id());

    PhysicsZone::AddPhysicsEntity(entity, layer);
}

void TestZone::RemovePhysicsEntity(const mono::IPhysicsEntityPtr& entity)
{
    const bool damagable = entity->HasProperty(EntityProperties::DAMAGABLE);
    if(damagable)
        m_damageController.RemoveRecord(entity->Id());

    PhysicsZone::RemovePhysicsEntity(entity);
}

void TestZone::AddEntity(const mono::IEntityPtr& entity, int layer)
{
    PhysicsZone::AddEntity(entity, layer);

    const bool damagable = entity->HasProperty(EntityProperties::DAMAGABLE);
    if(damagable)
        m_damageController.CreateRecord(entity->Id());
}

void TestZone::RemoveEntity(const mono::IEntityPtr& entity)
{
    PhysicsZone::RemoveEntity(entity);

    const bool damagable = entity->HasProperty(EntityProperties::DAMAGABLE);
    if(damagable)
        m_damageController.RemoveRecord(entity->Id());
}




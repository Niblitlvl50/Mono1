
#include "TestZone.h"
#include "Rendering/ICamera.h"
#include "Audio/AudioFactory.h"

#include "Factories.h"
#include "Enemies/Enemy.h"
#include "Enemies/IEnemyFactory.h"

#include "Effects/SmokeEffect.h"

#include "Shuttle.h"
#include "Explosion.h"

#include "EventHandler/EventHandler.h"
#include "Events/GameEventFuncFwd.h"
#include "Events/SpawnEntityEvent.h"
#include "Events/SpawnPhysicsEntityEvent.h"
#include "Events/RemoveEntityEvent.h"
#include "Events/ShockwaveEvent.h"
#include "Events/DamageEvent.h"
#include "Events/SpawnConstraintEvent.h"

#include "Math/MathFunctions.h"
#include "Utils.h"
#include "EntityProperties.h"
#include "RenderLayers.h"

#include "Paths/PathFactory.h"

#include "WorldFile.h"
#include "World.h"

#include "UpdateTasks/ListenerPositionUpdater.h"
#include "UpdateTasks/HealthbarUpdater.h"
#include "UpdateTasks/CameraViewportReporter.h"

#include "Rendering/IRenderer.h"
#include "Rendering/Color.h"
#include "Math/Matrix.h"
#include "Math/Quad.h"
#include "FontIds.h"

namespace
{
    class FPSEntity : public mono::EntityBase
    {
    public:
    
        FPSEntity()
        {
            m_projection = math::Ortho(0.0f, 600.0f, 0.0f, 400.0f, -10.0f, 10.0f);
        }

        virtual void Draw(mono::IRenderer& renderer) const
        {
            constexpr math::Matrix identity;
            renderer.PushNewTransform(identity);
            renderer.PushNewProjection(m_projection);

            char text[32] = { '\0' };
            std::snprintf(text, 32, "fps: %u frames: %u", m_counter.Fps(), m_counter.Frames());

            constexpr mono::Color::RGBA color(1, 0, 0, 1);
            renderer.DrawText(game::LARGE, text, math::Vector(10, 10), false, color);
        }

        virtual void Update(unsigned int delta)
        {
            m_counter++;
        }

        virtual math::Quad BoundingBox() const
        {
            return math::Quad(-math::INF, -math::INF, math::INF, math::INF);
        }

        mono::FPSCounter m_counter;
        math::Matrix m_projection;
    };
}

using namespace game;

TestZone::TestZone(mono::EventHandler& eventHandler)
    : PhysicsZone(math::Vector(0.0f, 0.0f), 0.9f),
      mEventHandler(eventHandler),
      m_spawner(eventHandler)
{
    using namespace std::placeholders;
    
    const game::SpawnEntityFunc& spawnEntityFunc = std::bind(&TestZone::SpawnEntity, this, _1);
    const game::SpawnPhysicsEntityFunc& spawnPhysicsFunc = std::bind(&TestZone::SpawnPhysicsEntity, this, _1);
    const game::RemoveEntityFunc& removeFunc = std::bind(&TestZone::OnRemoveEntity, this, _1);
    const game::ShockwaveFunc& shockwaveFunc = std::bind(&TestZone::OnShockwaveEvent, this, _1);
    const game::DamageFunc& damageFunc = std::bind(&TestZone::OnDamageEvent, this, _1);
    const game::SpawnConstraintFunc& constraintFunc = std::bind(&TestZone::OnSpawnConstraint, this, _1);
    const game::DespawnConstraintFunc& despawnConstraintFunc = std::bind(&TestZone::OnDespawnConstraint, this, _1);

    mSpawnEntityToken = mEventHandler.AddListener(spawnEntityFunc);
    mSpawnPhysicsEntityToken = mEventHandler.AddListener(spawnPhysicsFunc);
    mRemoveEntityByIdToken = mEventHandler.AddListener(removeFunc);
    mShockwaveEventToken = mEventHandler.AddListener(shockwaveFunc);
    mDamageEventToken = mEventHandler.AddListener(damageFunc);
    m_spawnConstraintToken = mEventHandler.AddListener(constraintFunc);
    m_despawnConstraintToken = mEventHandler.AddListener(despawnConstraintFunc);

    m_backgroundMusic = mono::AudioFactory::CreateSound("sound/ingame_phoenix.wav", true, true);
}

TestZone::~TestZone()
{
    mEventHandler.RemoveListener(mSpawnEntityToken);
    mEventHandler.RemoveListener(mSpawnPhysicsEntityToken);
    mEventHandler.RemoveListener(mRemoveEntityByIdToken);
    mEventHandler.RemoveListener(mShockwaveEventToken);
    mEventHandler.RemoveListener(mDamageEventToken);
    mEventHandler.RemoveListener(m_spawnConstraintToken);
    mEventHandler.RemoveListener(m_despawnConstraintToken);
}

void TestZone::OnLoad(mono::ICameraPtr camera)
{
    File::FilePtr world_file = File::OpenBinaryFile("world.world");
    
    world::LevelFileHeader world_header;
    world::ReadWorld(world_file, world_header);
    game::LoadWorld(this, world_header.polygons);

    std::shared_ptr<Shuttle> shuttle = std::make_shared<Shuttle>(math::zeroVec, mEventHandler);

    AddUpdatable(std::make_shared<ListenerPositionUpdater>(shuttle));
    AddUpdatable(std::make_shared<CameraViewportReporter>(camera));
    AddUpdatable(std::make_shared<HealthbarUpdater>(m_healthbars, m_damageController, *this));
    AddDrawable(std::make_shared<HealthbarDrawer>(m_healthbars), FOREGROUND);
    AddPhysicsEntity(shuttle, FOREGROUND);

    AddPhysicsEntity(enemy_factory->CreateCacoDemon(math::Vector(100, 100)), FOREGROUND);
    AddPhysicsEntity(enemy_factory->CreateInvader(math::Vector(20.0f, 100.0f)), MIDDLEGROUND);
    AddPhysicsEntity(enemy_factory->CreateInvader(math::Vector(20.0f, 100.0f)), MIDDLEGROUND);
    AddPhysicsEntity(enemy_factory->CreateInvader(math::Vector(20.0f, 100.0f)), MIDDLEGROUND);
    AddPhysicsEntity(enemy_factory->CreateInvader(math::Vector(20.0f, 100.0f)), MIDDLEGROUND);
    AddPhysicsEntity(enemy_factory->CreateInvader(math::Vector(20.0f, 100.0f)), MIDDLEGROUND);

    AddPhysicsEntity(enemy_factory->CreateBlackSquare(math::Vector(-30.0f, 70.0f)), MIDDLEGROUND);
    AddPhysicsEntity(enemy_factory->CreateBlackSquare(math::Vector(-30.0f, 70.0f)), MIDDLEGROUND);
    AddPhysicsEntity(enemy_factory->CreateBlackSquare(math::Vector(-30.0f, 70.0f)), MIDDLEGROUND);

    const mono::IPathPtr& path = mono::CreatePath("paths/smaller_loop.path");
    AddPhysicsEntity(enemy_factory->CreatePathInvader(path), MIDDLEGROUND);
    AddPhysicsEntity(enemy_factory->CreatePathInvader(path), MIDDLEGROUND);
    AddPhysicsEntity(enemy_factory->CreatePathInvader(path), MIDDLEGROUND);

    AddEntity(std::make_shared<SmokeEffect>(math::Vector(-10.0f, 10.0f)), BACKGROUND);
    AddEntity(std::make_shared<FPSEntity>(), FOREGROUND);

    camera->SetPosition(shuttle->Position());
    camera->Follow(shuttle, math::Vector(0, -4));

    m_backgroundMusic->Play();
}

void TestZone::OnUnload()
{ }

bool TestZone::SpawnEntity(const game::SpawnEntityEvent& event)
{
    AddEntity(event.entity, FOREGROUND);
    return true;
}

bool TestZone::SpawnPhysicsEntity(const game::SpawnPhysicsEntityEvent& event)
{
    AddPhysicsEntity(event.entity, FOREGROUND);
    return true;
}

bool TestZone::OnRemoveEntity(const game::RemoveEntityEvent& event)
{
    mono::IPhysicsEntityPtr physics_entity = FindPhysicsEntityFromId(event.id);
    if(physics_entity)
    {
        SchedulePreFrameTask(std::bind(&TestZone::RemovePhysicsEntity, this, physics_entity));
        return true;
    }

    mono::IEntityPtr entity = FindEntityFromId(event.id);
    if(entity)
        SchedulePreFrameTask(std::bind(&TestZone::RemoveEntity, this, entity));

    return true;
}

bool TestZone::OnShockwaveEvent(const game::ShockwaveEvent& event)
{
    const auto shockwave_func = [&event](const mono::IBodyPtr& body) {
        math::Vector unit = body->GetPosition() - event.position;
        const float length = math::Length(unit);
        if(length > 4)
            return;

        math::Normalize(unit);
        
        const math::Vector& impulse = unit * event.magnitude;
        body->ApplyImpulse(impulse, body->GetPosition());
    };

    PhysicsZone::ForEachBody(shockwave_func);

    return true;
}

bool TestZone::OnDamageEvent(const game::DamageEvent& event)
{
    mono::IPhysicsEntityPtr entity = FindPhysicsEntityFromBody(event.body);
    if(!entity)
        return false;

    const DamageResult& result = m_damageController.ApplyDamage(entity->Id(), event.damage);
    if(!result.success)
        return false;

    if(result.health_left <= 0)
    {
        m_damageController.RemoveRecord(entity->Id());
        SchedulePreFrameTask(std::bind(&TestZone::RemovePhysicsEntity, this, entity));

        game::ExplosionConfiguration config;
        config.position = entity->Position();
        config.scale = 1.5f;
        //config.rotation = mono::Random(0.0f, math::PI() * 2.0f);
        config.sprite_file = "sprites/explosion.sprite";

        AddEntity(std::make_shared<Explosion>(config, mEventHandler), FOREGROUND);
    }

    return true;
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

bool TestZone::OnSpawnConstraint(const game::SpawnConstraintEvent& event)
{
    PhysicsZone::AddConstraint(event.constraint);
    return true;
}

bool TestZone::OnDespawnConstraint(const game::DespawnConstraintEvent& event)
{
    PhysicsZone::RemoveConstraint(event.constraint);
    return true;
}

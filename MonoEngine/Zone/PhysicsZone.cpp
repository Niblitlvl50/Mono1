

#include "PhysicsZone.h"
#include "Rendering/IRenderer.h"
#include "IUpdatable.h"
#include "Physics/CMSpace.h"
#include "Physics/CMPhysicsData.h"
#include "Physics/IShape.h"
#include "Entity/IPhysicsEntity.h"

#include "Utils.h"
#include <cstdio>

using namespace mono;

struct PhysicsZone::PhysicsImpl : IUpdatable
{
    PhysicsImpl(const math::Vector& gravity, float damping)
        : m_space(gravity, damping)
    { }
    void doUpdate(unsigned int delta)
    {
        m_space.Tick(delta);
    }
    mono::PhysicsSpace m_space;
};

PhysicsZone::PhysicsZone(const math::Vector& gravity, float damping)
    : m_physics(new PhysicsImpl(gravity, damping))
{
    // Add space physics as an updatable
    AddUpdatable(m_physics);
}

void PhysicsZone::ForEachBody(const mono::BodyFunc& func)
{
    m_physics->m_space.ForEachBody(func);
}

void PhysicsZone::AddPhysicsEntity(const mono::IPhysicsEntityPtr& entity, int layer)
{
    AddDrawable(entity, layer);
    AddUpdatable(entity);
    AddPhysicsData(entity->GetPhysics());

    m_physics_entities.push_back(entity);
}

void PhysicsZone::RemovePhysicsEntity(const mono::IPhysicsEntityPtr& entity)
{
    RemoveDrawable(entity);
    RemoveUpdatable(entity);

    const bool result = mono::remove(m_physics_entities, entity);
    if(result)
        RemovePhysicsData(entity->GetPhysics());
    else
        std::printf("PhysicsZone - Unable to remove physics entity\n");
}

void PhysicsZone::AddConstraint(const mono::IConstraintPtr& constraint)
{
    m_physics->m_space.Add(constraint);
    m_constraints.push_back(constraint);
}

void PhysicsZone::RemoveConstraint(const mono::IConstraintPtr& constraint)
{
    m_physics->m_space.Remove(constraint);
    mono::remove(m_constraints, constraint);
}

void PhysicsZone::AddPhysicsData(const mono::PhysicsData& physics_data)
{
    m_physics->m_space.Add(physics_data.body);
    
    for(auto& shape : physics_data.shapes)
        m_physics->m_space.Add(shape);
}

void PhysicsZone::RemovePhysicsData(const mono::PhysicsData& physics_data)
{
    m_physics->m_space.Remove(physics_data.body);
    
    for(auto& shape : physics_data.shapes)
        m_physics->m_space.Remove(shape);
}

IPhysicsEntityPtr PhysicsZone::FindPhysicsEntityFromBody(const mono::IBodyPtr& body) const
{
    for(auto& entity : m_physics_entities)
    {
        if(entity->GetPhysics().body == body)
            return entity;
    }

    return nullptr;
}

IPhysicsEntityPtr PhysicsZone::FindPhysicsEntityFromId(unsigned int id) const
{
    for(auto& entity : m_physics_entities)
    {
        if(entity->Id() == id)
            return entity;
    }

    return nullptr;
}




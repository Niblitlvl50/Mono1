

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
        : mSpace(gravity, damping)
    { }
    void doUpdate(unsigned int delta)
    {
        const float floatDelta = delta;
        mSpace.Tick(floatDelta / 1000.0f);
    }
    mono::Space mSpace;
};

PhysicsZone::PhysicsZone(const math::Vector& gravity, float damping)
    : mPhysics(new PhysicsImpl(gravity, damping))
{
    // Add space physics as an updatable
    AddUpdatable(mPhysics);
}

void PhysicsZone::ForEachBody(const mono::BodyFunc& func)
{
    mPhysics->mSpace.ForEachBody(func);
}

void PhysicsZone::AddPhysicsEntity(const mono::IPhysicsEntityPtr& entity, int layer)
{
    AddDrawable(entity, layer);
    AddUpdatable(entity);
    AddPhysicsData(entity->GetPhysics());

    mPhysicsEntities.push_back(entity);
}

void PhysicsZone::RemovePhysicsEntity(const mono::IPhysicsEntityPtr& entity)
{
    RemoveDrawable(entity);
    RemoveUpdatable(entity);

    const bool result = FindAndRemove(mPhysicsEntities, entity);
    if(result)
        RemovePhysicsData(entity->GetPhysics());
    else
        std::printf("PhysicsZone - Unable to remove physics entity\n");
        //throw std::runtime_error("PhysicsZone - Unable to remove physics entity");
}

void PhysicsZone::AddConstraint(const mono::IConstraintPtr& constraint)
{
    mPhysics->mSpace.Add(constraint);
    m_constraints.push_back(constraint);
}

void PhysicsZone::RemoveConstraint(const mono::IConstraintPtr& constraint)
{
    mPhysics->mSpace.Remove(constraint);
    mono::FindAndRemove(m_constraints, constraint);
}

void PhysicsZone::AddPhysicsData(const mono::PhysicsData& physics_data)
{
    mPhysics->mSpace.Add(physics_data.body);
    
    for(auto& shape : physics_data.shapes)
        mPhysics->mSpace.Add(shape);
}

void PhysicsZone::RemovePhysicsData(const mono::PhysicsData& physics_data)
{
    mPhysics->mSpace.Remove(physics_data.body);
    
    for(auto& shape : physics_data.shapes)
        mPhysics->mSpace.Remove(shape);
}

IPhysicsEntityPtr PhysicsZone::FindPhysicsEntityFromBody(const mono::IBodyPtr& body) const
{
    for(auto& entity : mPhysicsEntities)
    {
        if(entity->GetPhysics().body == body)
            return entity;
    }

    return nullptr;
}

IPhysicsEntityPtr PhysicsZone::FindPhysicsEntityFromId(unsigned int id) const
{
    for(auto& entity : mPhysicsEntities)
    {
        if(entity->Id() == id)
            return entity;
    }

    return nullptr;
}




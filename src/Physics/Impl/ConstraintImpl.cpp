
#include "ConstraintImpl.h"
#include "Physics/IBody.h"
#include "Physics/PhysicsSystem.h"
#include "Physics/PhysicsSpace.h"

#include "chipmunk/chipmunk.h"

using namespace cm;

ConstraintImpl::ConstraintImpl()
    : m_physics_system(nullptr)
    , m_constraint(nullptr)
{ }

ConstraintImpl::ConstraintImpl(cpConstraint* constraint)
    : m_constraint(constraint)
    , m_physics_system(nullptr)
{ }

void ConstraintImpl::Init(mono::PhysicsSystem* physics_system)
{
    m_physics_system = physics_system;
}

void ConstraintImpl::SetHandle(cpConstraint* constraint)
{
    m_constraint = constraint;
}

void ConstraintImpl::SetMaxForce(float force) 
{
    cpConstraintSetMaxForce(m_constraint, force);
}

float ConstraintImpl::GetMaxForce() const 
{
    return cpConstraintGetMaxForce(m_constraint);
}

void ConstraintImpl::SetMaxBias(float bias) 
{
    cpConstraintSetMaxBias(m_constraint, bias);
}

float ConstraintImpl::GetMaxBias() const 
{
    return cpConstraintGetMaxBias(m_constraint);
}

void ConstraintImpl::SetErrorBias(float bias) 
{
    cpConstraintSetErrorBias(m_constraint, bias);
}

float ConstraintImpl::GetErrorBias() const 
{
    return cpConstraintGetErrorBias(m_constraint);
}

float ConstraintImpl::GetAppliedImpulse() const
{
    return cpConstraintGetImpulse(m_constraint);
}

float ConstraintImpl::GetAppliedForce() const
{
    const mono::PhysicsSpace* space = m_physics_system->GetSpace();
    const float applied_force = GetAppliedImpulse() / space->GetTimeStep();
    return applied_force;
}

float ConstraintImpl::GetAppliedForcePercentage() const
{
    return GetAppliedForce() / GetMaxForce();
}

mono::ConstraintBodyPair ConstraintImpl::GetBodies()
{
    return {
        (mono::IBody*)cpBodyGetUserData(cpConstraintGetBodyA(m_constraint)),
        (mono::IBody*)cpBodyGetUserData(cpConstraintGetBodyB(m_constraint))
    };
}

cpConstraint* ConstraintImpl::Handle() 
{
    return m_constraint;
}

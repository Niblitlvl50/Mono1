
#include "ConstraintImpl.h"
#include "Physics/IBody.h"

#include "chipmunk/chipmunk.h"

using namespace cm;

ConstraintImpl::ConstraintImpl()
    : m_constraint(nullptr)
{ }

ConstraintImpl::ConstraintImpl(cpConstraint* constraint)
    : m_constraint(constraint)
{ }

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

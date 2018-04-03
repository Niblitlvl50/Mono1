
#include "ConstraintImpl.h"

#include "chipmunk/chipmunk.h"

using namespace cm;

ConstraintImpl::ConstraintImpl(cpConstraint* constraint)
    : m_constraint(constraint)
{ }

ConstraintImpl::~ConstraintImpl()
{
    cpConstraintFree(m_constraint);
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

cpConstraint* ConstraintImpl::Handle() 
{
    return m_constraint;
}

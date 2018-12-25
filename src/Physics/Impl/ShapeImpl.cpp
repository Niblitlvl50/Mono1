
#include "ShapeImpl.h"
#include "chipmunk/chipmunk.h"

using namespace cm;

ShapeImpl::ShapeImpl(cpShape* shape, float inertia_value)
    : m_shape(shape)
    , m_inertia_value(inertia_value)
{ }

void ShapeImpl::SetElasticity(float value) 
{
    cpShapeSetElasticity(m_shape, value);
}

void ShapeImpl::SetFriction(float value) 
{
    cpShapeSetFriction(m_shape, value);
}

float ShapeImpl::GetInertiaValue() const 
{
    return m_inertia_value;
}

void ShapeImpl::SetCollisionFilter(unsigned int category, unsigned int mask) 
{
    cpShapeFilter filter = cpShapeGetFilter(m_shape);
    filter.categories = category;
    filter.mask = mask;
    cpShapeSetFilter(m_shape, filter);
}

cpShape* ShapeImpl::Handle() 
{
    return m_shape;
}

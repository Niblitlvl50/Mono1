
#include "ShapeImpl.h"
#include "chipmunk/chipmunk.h"

using namespace cm;

ShapeImpl::ShapeImpl()
    : m_shape(nullptr)
    , m_inertia_value(0.0f)
{ }

ShapeImpl::ShapeImpl(cpShape* shape, float inertia_value)
    : m_shape(shape)
    , m_inertia_value(inertia_value)
{ }

void ShapeImpl::SetShapeHandle(cpShape* shape)
{
    m_shape = shape;
}

void ShapeImpl::SetElasticity(float value) 
{
    cpShapeSetElasticity(m_shape, value);
}

void ShapeImpl::SetFriction(float value) 
{
    cpShapeSetFriction(m_shape, value);
}

void ShapeImpl::SetInertia(float inertia)
{
    m_inertia_value = inertia;
}

float ShapeImpl::GetInertiaValue() const 
{
    return m_inertia_value;
}

void ShapeImpl::SetSensor(bool is_sensor)
{
    cpShapeSetSensor(m_shape, is_sensor);
}

void ShapeImpl::SetCollisionFilter(uint32_t category, uint32_t mask) 
{
    cpShapeFilter filter = cpShapeGetFilter(m_shape);
    filter.categories = category;
    filter.mask = mask;
    cpShapeSetFilter(m_shape, filter);
}

void ShapeImpl::SetCollisionMask(uint32_t mask)
{
    cpShapeFilter filter = cpShapeGetFilter(m_shape);
    filter.mask = mask;
    cpShapeSetFilter(m_shape, filter);
}

cpShape* ShapeImpl::Handle() 
{
    return m_shape;
}

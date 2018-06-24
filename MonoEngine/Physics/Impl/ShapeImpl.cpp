
#include "ShapeImpl.h"
#include "Physics/IBody.h"
#include "Math/Vector.h"
#include "Math/MathFunctions.h"

#include "chipmunk/chipmunk.h"
#include <algorithm>

using namespace cm;

ShapeImpl::ShapeImpl(mono::IBodyPtr& body, float radius, const math::Vector& offset)
{
    m_shape = cpCircleShapeNew(body->Handle(), radius, cpv(offset.x, offset.y));
    m_inertia_value = cpMomentForCircle(body->GetMass(), 0.0f, radius, cpv(offset.x, offset.y));
}

ShapeImpl::ShapeImpl(mono::IBodyPtr& body, float width, float height)
{
    m_shape = cpBoxShapeNew(body->Handle(), width, height, 0.0f);
    m_inertia_value = cpMomentForBox(body->GetMass(), width, height);
}

ShapeImpl::ShapeImpl(mono::IBodyPtr& body, const math::Vector& first, const math::Vector& second, float radius)
{
    m_shape = cpSegmentShapeNew(body->Handle(), cpv(first.x, first.y), cpv(second.x, second.y), radius);
    m_inertia_value = cpMomentForSegment(body->GetMass(), cpv(first.x, first.y), cpv(second.x, second.y), radius);
}

ShapeImpl::ShapeImpl(mono::IBodyPtr& body, const std::vector<math::Vector>& vertices, const math::Vector& offset)
{
    const auto transformFunc = [](const math::Vector& position) {
        return cpv(position.x, position.y);
    };
    
    std::vector<cpVect> vects(vertices.size());
    std::transform(vertices.begin(), vertices.end(), vects.begin(), transformFunc);

    const bool clockwice = math::IsPolygonClockwise(vertices);
    if(clockwice)
        std::reverse(vects.begin(), vects.end());

    m_shape = cpPolyShapeNewRaw(body->Handle(), int(vects.size()), vects.data(), 0.1f);
    m_inertia_value = cpMomentForPoly(body->GetMass(), int(vects.size()), vects.data(), cpv(offset.x, offset.y), 1.0f);
}

ShapeImpl::~ShapeImpl()
{
    cpShapeFree(m_shape);
}

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

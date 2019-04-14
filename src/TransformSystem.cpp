
#include "TransformSystem.h"
#include "Hash.h"
#include <numeric>
#include <cstdint>

using namespace mono;

namespace
{
    constexpr uint32_t no_parent = std::numeric_limits<uint32_t>::max();
}

TransformSystem::TransformSystem(size_t n_components)
{
    m_transforms.resize(n_components);
    for(Component& component : m_transforms)
    {
        component.parent = no_parent;
        component.bounding_box = math::Quad(-0.5f, -0.5f, 0.5f, 0.5f);
    }
}

math::Matrix TransformSystem::GetWorld(uint32_t id) const
{
    uint32_t current_id = id;
    math::Matrix transform;

    while(current_id != no_parent)
    {
        const Component& transform_component = m_transforms[current_id];

        transform = transform * transform_component.transform;
        current_id = transform_component.parent;
    }

    return transform;
}

const math::Matrix& TransformSystem::GetTransform(uint32_t id) const
{
    return m_transforms[id].transform;
}

math::Matrix& TransformSystem::GetTransform(uint32_t id)
{
    return m_transforms[id].transform;
}

math::Quad TransformSystem::GetWorldBoundingBox(uint32_t id) const
{
    math::Quad bounding_box;
    bounding_box.mA = math::Vector(100000, 100000);
    bounding_box.mB = math::Vector(-100000, -100000);

    while(id != no_parent)
    {
        const Component& transform_component = m_transforms[id];
        bounding_box |= math::Transform(transform_component.transform, transform_component.bounding_box);

        //transform = transform * transform_component.transform;
        id = transform_component.parent;
    }

    return bounding_box;
}

const math::Quad& TransformSystem::GetBoundingBox(uint32_t id) const
{
    return m_transforms[id].bounding_box;
}

math::Quad& TransformSystem::GetBoundingBox(uint32_t id)
{
    return m_transforms[id].bounding_box;
}

uint32_t TransformSystem::GetParent(uint32_t id) const
{
    return m_transforms[id].parent;
}

void TransformSystem::ChildTransform(uint32_t id, uint32_t parent)
{
    m_transforms[id].parent = parent;
}

void TransformSystem::UnchildTransform(uint32_t id)
{
    m_transforms[id].parent = no_parent;
}

uint32_t TransformSystem::Id() const
{
    return mono::Hash(Name());
}

const char* TransformSystem::Name() const
{
    return "transformsystem";
}

uint32_t TransformSystem::Capacity() const
{
    return m_transforms.size();
}

void TransformSystem::Update(uint32_t delta_ms)
{

}

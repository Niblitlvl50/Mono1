
#include "EntityProxy.h"
#include "Grabber.h"
#include "SpriteEntity.h"
#include "Math/Quad.h"
#include "Math/MathFunctions.h"

using namespace editor;

EntityProxy::EntityProxy(const std::shared_ptr<SpriteEntity>& entity)
    : m_entity(entity)
{ }

uint EntityProxy::Id() const
{
    return m_entity->Id();
}

mono::IEntityPtr EntityProxy::Entity()
{
    return m_entity;
}

void EntityProxy::SetSelected(bool selected)
{
    m_entity->SetSelected(selected);
}

bool EntityProxy::Intersects(const math::Vector& position) const
{
    const math::Quad& bb = m_entity->BoundingBox();
    return math::PointInsideQuad(position, bb);
}

std::vector<Grabber> EntityProxy::GetGrabbers() const
{
    return std::vector<Grabber>();
}

void EntityProxy::UpdateUIContext(UIContext& context) const
{

}

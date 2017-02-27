
#include "EntityProxy.h"
#include "Grabber.h"
#include "SpriteEntity.h"
#include "UIContext.h"

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
    context.components = UIComponent::NAME | UIComponent::POSITIONAL;

    const math::Vector& position = m_entity->Position();

    context.name = m_entity->Name().c_str();
    context.position_x = position.x;
    context.position_y = position.y;
    context.rotation = m_entity->Rotation();
}

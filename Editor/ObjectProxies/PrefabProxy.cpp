
#include "PrefabProxy.h"
#include "Prefab.h"
#include "Grabber.h"
#include "UIContext.h"

#include "Math/Quad.h"
#include "Math/MathFunctions.h"

using namespace editor;

PrefabProxy::PrefabProxy(const std::shared_ptr<Prefab>& prefab)
    : m_prefab(prefab)
{ }

uint PrefabProxy::Id() const
{
    return m_prefab->Id();
}

mono::IEntityPtr PrefabProxy::Entity()
{
    return m_prefab;
}

void PrefabProxy::SetSelected(bool selected)
{
    m_prefab->SetSelected(selected);
}

bool PrefabProxy::Intersects(const math::Vector& position) const
{
    const math::Quad& bb = m_prefab->BoundingBox();
    return math::PointInsideQuad(position, bb);
}

std::vector<Grabber> PrefabProxy::GetGrabbers() const
{
    return std::vector<Grabber>();
}

void PrefabProxy::UpdateUIContext(UIContext& context) const
{
    context.components = UIComponent::NAME | UIComponent::POSITIONAL;

    context.name = m_prefab->Name().c_str();
    context.position = m_prefab->Position();
    context.rotation = m_prefab->Rotation();
}


#include "PrefabProxy.h"
#include "Prefab.h"
#include "Grabber.h"
#include "SnapPoint.h"
#include "UIContext.h"

#include "Math/Quad.h"
#include "Math/Matrix.h"
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

std::vector<SnapPoint> PrefabProxy::GetSnappers() const
{
    math::Matrix matrix = m_prefab->Transformation();
    math::Inverse(matrix);

    const auto func = [&matrix](SnapPoint& point) {
        point.position = math::Transform(matrix, point.position);
    };

    std::vector<SnapPoint> snappers = m_prefab->SnapPoints();
    std::for_each(snappers.begin(), snappers.end(), func);

    return snappers;
}

void PrefabProxy::UpdateUIContext(UIContext& context) const
{
    context.components = UIComponent::NAME | UIComponent::POSITIONAL;

    context.name = m_prefab->Name().c_str();
    context.position = m_prefab->Position();
    context.rotation = m_prefab->Rotation();
}

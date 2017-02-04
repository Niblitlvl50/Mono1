
#include "PathProxy.h"
#include "Grabber.h"
#include "Path.h"
#include "Math/Matrix.h"
#include "Math/Quad.h"
#include "Math/MathFunctions.h"

using namespace editor;

PathProxy::PathProxy(const std::shared_ptr<PathEntity>& path)
    : m_path(path)
{ }

uint PathProxy::Id() const
{
    return m_path->Id();
}

mono::IEntityPtr PathProxy::Entity()
{
    return m_path;
}

void PathProxy::SetSelected(bool selected)
{
    m_path->SetSelected(selected);
}

bool PathProxy::Intersects(const math::Vector& position) const
{
    const math::Quad& bb = m_path->BoundingBox();
    return math::PointInsideQuad(position, bb);
}

std::vector<Grabber> PathProxy::GetGrabbers() const
{
    using namespace std::placeholders;

    const math::Matrix& transform = m_path->Transformation();
    const auto& vertices = m_path->m_points;

    std::vector<Grabber> grabbers;
    grabbers.reserve(vertices.size());

    for(size_t index = 0; index < vertices.size(); ++index)
    {
        Grabber grab;
        grab.position = math::Transform(transform, vertices[index]);
        grab.callback = std::bind(&PathEntity::SetVertex, m_path, _1, index);
        grabbers.push_back(grab);
    }

    return grabbers;
}

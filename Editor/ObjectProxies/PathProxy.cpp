
#include "PathProxy.h"
#include "Grabber.h"
#include "SnapPoint.h"
#include "UIContext.h"
#include "Path.h"
#include "Editor.h"
#include "Math/Matrix.h"
#include "Math/Quad.h"
#include "Math/MathFunctions.h"

using namespace editor;

PathProxy::PathProxy(const std::shared_ptr<PathEntity>& path, Editor* editor)
    : m_path(path),
      m_editor(editor)
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
    const bool inside_bb = math::PointInsideQuad(position, bb);
    if(inside_bb)
    {
        math::Matrix transform = m_path->Transformation();
        math::Inverse(transform);
        const math::Vector& local_position = math::Transform(transform, position);

        float min_distance = math::INF;

        const auto& vertices = m_path->m_points;
        for(size_t index = 0; index < vertices.size() -1; ++index)
        {
            const math::Vector& line_point = math::ClosestPointOnLine(vertices[index], vertices[index+1], local_position);
            const float length = math::Length(line_point - local_position);
            min_distance = std::min(min_distance, length);
        }

        const float threshold = m_editor->GetPickingDistance();
        return (min_distance < threshold);
    }

    return false;
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

std::vector<SnapPoint> PathProxy::GetSnappers() const
{
    return std::vector<SnapPoint>();
}

void PathProxy::UpdateUIContext(UIContext& context) const
{
    context.components =
        UIComponent::NAME |
        UIComponent::NAME_EDITABLE |
        UIComponent::POSITIONAL;

    using namespace std::placeholders;
    context.name_callback = std::bind(&PathEntity::SetName, m_path, _1);

    context.name = m_path->m_name.c_str();
    context.position = m_path->Position();
    context.rotation = 0.0f;
}

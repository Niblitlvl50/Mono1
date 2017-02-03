
#include "PathProxy.h"
#include "Grabber.h"
#include "Editor.h"
#include "Path.h"
#include "Math/Matrix.h"
#include "Math/MathFunctions.h"

using namespace editor;

namespace
{
    std::shared_ptr<PathEntity> FindPath(uint id, std::vector<std::shared_ptr<PathEntity>>& paths)
    {
        const auto find_path_func = [id](const std::shared_ptr<editor::PathEntity>& path) {
            return path->Id() == id;
        };

        const auto it = std::find_if(paths.begin(), paths.end(), find_path_func);
        if(it != paths.end())
           return *it;
        
        return nullptr;
    }
}

PathProxy::PathProxy(uint id, Editor* editor)
    : m_id(id),
      m_editor(editor)
{ }

uint PathProxy::Id() const
{
    return m_id;
}

void PathProxy::SetSelected(bool selected)
{
    auto path = FindPath(m_id, m_editor->m_paths);
    if(path)
        path->SetSelected(selected);
}

bool PathProxy::Intersects(const math::Vector& position) const
{
    auto path = FindPath(m_id, m_editor->m_paths);
    if(!path)
        return false;

    const math::Quad& bb = path->BoundingBox();
    return math::PointInsideQuad(position, bb);
}

std::vector<Grabber> PathProxy::GetGrabbers() const
{
    using namespace std::placeholders;

    std::vector<Grabber> grabbers;

    auto path = FindPath(m_id, m_editor->m_paths);
    if(!path)
        return grabbers;

    const math::Matrix& transform = path->Transformation();

    const auto& vertices = path->m_points;
    for(size_t index = 0; index < vertices.size(); ++index)
    {
        Grabber grab;
        grab.position = math::Transform(transform, vertices[index]);
        grab.callback = std::bind(&PathEntity::SetVertex, path, _1, index);
        grabbers.push_back(grab);
    }

    return grabbers;
}

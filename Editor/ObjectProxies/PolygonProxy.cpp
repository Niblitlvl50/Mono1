
#include "PolygonProxy.h"
#include "Grabber.h"
#include "Editor.h"
#include "Polygon.h"
#include "Math/MathFunctions.h"
#include "Math/Matrix.h"

using namespace editor;

namespace
{
    std::shared_ptr<PolygonEntity> FindPolygon(uint id, std::vector<std::shared_ptr<PolygonEntity>>& polygons)
    {
        const auto find_func = [id](const std::shared_ptr<editor::PolygonEntity>& entity) {
            return entity->Id() == id;
        };

        const auto it = std::find_if(polygons.begin(), polygons.end(), find_func);
        if(it != polygons.end())
           return *it;

        return nullptr;
    }
}

PolygonProxy::PolygonProxy(uint id, Editor* editor)
    : m_id(id),
      m_editor(editor)
{ }

uint PolygonProxy::Id() const
{
    return m_id;
}

void PolygonProxy::SetSelected(bool selected)
{
    auto polygon = FindPolygon(m_id, m_editor->m_polygons);
    if(polygon)
        polygon->SetSelected(selected);
}

bool PolygonProxy::Intersects(const math::Vector& position) const
{
    auto polygon = FindPolygon(m_id, m_editor->m_polygons);
    if(!polygon)
        return false;
    
    const math::Quad& bb = polygon->BoundingBox();
    return math::PointInsideQuad(position, bb);
}

std::vector<Grabber> PolygonProxy::GetGrabbers() const
{
    using namespace std::placeholders;

    std::vector<Grabber> grabbers;

    auto polygon = FindPolygon(m_id, m_editor->m_polygons);
    if(!polygon)
        return grabbers;

    const math::Matrix& transform = polygon->Transformation();

    const auto& vertices = polygon->GetVertices();
    for(size_t index = 0; index < vertices.size(); ++index)
    {
        Grabber grab;
        grab.position = math::Transform(transform, vertices[index]);
        grab.callback = std::bind(&PolygonEntity::SetVertex, polygon, _1, index);
        grabbers.push_back(grab);
    }

    return grabbers;
}

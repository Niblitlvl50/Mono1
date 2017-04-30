
#pragma once

#include "ITool.h"
#include "Math/Vector.h"
#include <memory>
#include <vector>

namespace editor
{
    class Editor;
    class PolygonEntity;

    class PolygonBrushTool : public ITool
    {
    public:

        PolygonBrushTool(Editor* editor);

        virtual void Begin();
        virtual void End();
        virtual bool IsActive() const;
        virtual void HandleContextMenu(int menu_index);
        virtual void HandleMouseDown(const math::Vector& world_pos, mono::IEntityPtr entity);
        virtual void HandleMouseUp(const math::Vector& world_pos);
        virtual void HandleMousePosition(const math::Vector& world_pos);

        Editor* m_editor;
        bool m_painting;

        std::shared_ptr<editor::PolygonEntity> m_polygon;
        math::Vector m_previouslyAddedPoint;
        math::Vector m_previousPoint;
        float m_direction;

        class Visualizer;
        std::shared_ptr<Visualizer> m_visualizer;

        std::vector<math::Vector> m_drawnPoints;
    };
}

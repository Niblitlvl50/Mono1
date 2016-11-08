//
//  PolygonBrushTool.hpp
//  MonoiOS
//
//  Created by Niklas Damberg on 09/08/16.
//
//

#pragma once

#include "ITool.h"
#include "Math/Vector2f.h"
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
        virtual void HandleMouseDown(const math::Vector2f& world_pos, mono::IEntityPtr entity);
        virtual void HandleMouseUp(const math::Vector2f& world_pos);
        virtual void HandleMousePosition(const math::Vector2f& world_pos);

        Editor* m_editor;
        bool m_painting;

        std::shared_ptr<editor::PolygonEntity> m_polygon;
        math::Vector2f m_previouslyAddedPoint;
        math::Vector2f m_previousPoint;
        float m_direction;

        class Visualizer;
        std::shared_ptr<Visualizer> m_visualizer;

        std::vector<math::Vector2f> m_drawnPoints;
    };
}

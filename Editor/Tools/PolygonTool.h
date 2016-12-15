//
//  PolygonTool.hpp
//  MonoiOS
//
//  Created by Niklas Damberg on 02/07/16.
//
//

#pragma once

#include "ITool.h"
#include "Math/Vector.h"
#include <memory>

namespace editor
{
    class Editor;
    class PolygonEntity;

    class PolygonTool : public ITool
    {
    public:

        PolygonTool(Editor* editor);

        virtual void Begin();
        virtual void End();
        virtual bool IsActive() const;
        virtual void HandleContextMenu(int menu_index);
        virtual void HandleMouseDown(const math::Vector& world_pos, mono::IEntityPtr entity);
        virtual void HandleMouseUp(const math::Vector& world_pos);
        virtual void HandleMousePosition(const math::Vector& world_pos);

        Editor* m_editor;
        std::shared_ptr<editor::PolygonEntity> m_polygon;
        bool m_firstPoint;

        math::Vector m_lastAddedPoint;
        math::Vector m_mousePosition;

        class Visualizer;
        std::shared_ptr<Visualizer> m_visualizer;
    };
}

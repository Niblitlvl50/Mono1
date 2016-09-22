//
//  PolygonTool.hpp
//  MonoiOS
//
//  Created by Niklas Damberg on 02/07/16.
//
//

#pragma once

#include "ITool.h"
#include "Math/Vector2f.h"
#include <memory>

namespace editor
{
    class EditorZone;
    class PolygonEntity;

    class PolygonTool : public ITool
    {
    public:

        PolygonTool(EditorZone* editor);

        virtual void Begin();
        virtual void End();
        virtual bool IsActive() const;
        virtual void HandleMouseDown(const math::Vector2f& world_pos, mono::IEntityPtr entity);
        virtual void HandleMouseUp(const math::Vector2f& world_pos);
        virtual void HandleMousePosition(const math::Vector2f& world_pos);

        EditorZone* m_editor;
        std::shared_ptr<editor::PolygonEntity> m_polygon;
        bool m_firstPoint;

        math::Vector2f m_lastAddedPoint;
        math::Vector2f m_mousePosition;

        class Visualizer;
        std::shared_ptr<Visualizer> m_visualizer;
    };
}

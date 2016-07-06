//
//  TranslateTool.hpp
//  MonoiOS
//
//  Created by Niklas Damberg on 03/07/16.
//
//

#pragma once

#include "ITool.h"
#include "Vector2f.h"
#include <memory>

namespace editor
{
    class EditorZone;
    class PolygonEntity;

    class TranslateTool : public ITool
    {
    public:

        TranslateTool(EditorZone* editor);
        
        virtual void Begin();
        virtual void End();
        virtual bool IsActive() const;
        virtual void HandleMouseDown(const math::Vector2f& world_pos);
        virtual void HandleMouseUp(const math::Vector2f& world_pos);
        virtual void HandleMousePosition(const math::Vector2f& world_pos);

        EditorZone* m_editor;
        std::shared_ptr<editor::PolygonEntity> m_polygon;
        math::Vector2f m_beginTranslate;
        math::Vector2f m_positionDiff;
    };
}

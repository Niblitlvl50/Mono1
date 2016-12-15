//
//  TranslateTool.hpp
//  MonoiOS
//
//  Created by Niklas Damberg on 03/07/16.
//
//

#pragma once

#include "ITool.h"
#include "Math/Vector.h"
#include <memory>

namespace editor
{
    class Editor;

    class TranslateTool : public ITool
    {
    public:

        TranslateTool(Editor* editor);

        virtual void Begin();
        virtual void End();
        virtual bool IsActive() const;
        virtual void HandleContextMenu(int menu_index);
        virtual void HandleMouseDown(const math::Vector& world_pos, mono::IEntityPtr entity);
        virtual void HandleMouseUp(const math::Vector& world_pos);
        virtual void HandleMousePosition(const math::Vector& world_pos);

    private:

        Editor* m_editor;
        mono::IEntityPtr m_entity;
        math::Vector m_beginTranslate;
        math::Vector m_positionDiff;
    };
}

//
//  TranslateTool.hpp
//  MonoiOS
//
//  Created by Niklas Damberg on 03/07/16.
//
//

#pragma once

#include "ITool.h"
#include "Math/Vector2f.h"
#include <memory>

namespace editor
{
    class TranslateTool : public ITool
    {
    public:

        virtual void Begin();
        virtual void End();
        virtual bool IsActive() const;
        virtual void HandleContextMenu(int menu_index);
        virtual void HandleMouseDown(const math::Vector2f& world_pos, mono::IEntityPtr entity);
        virtual void HandleMouseUp(const math::Vector2f& world_pos);
        virtual void HandleMousePosition(const math::Vector2f& world_pos);

        mono::IEntityPtr m_entity;
        math::Vector2f m_beginTranslate;
        math::Vector2f m_positionDiff;
    };
}

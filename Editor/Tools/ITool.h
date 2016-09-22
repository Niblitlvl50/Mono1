//
//  ITool.h
//  MonoiOS
//
//  Created by Niklas Damberg on 03/07/16.
//
//

#pragma once

#include "Math/MathFwd.h"
#include "MonoPtrFwd.h"

namespace editor
{
    class ITool
    {
    public:

        virtual ~ITool()
        { }

        virtual void Begin() = 0;
        virtual void End() = 0;
        virtual bool IsActive() const = 0;
        virtual void HandleMouseDown(const math::Vector2f& world_pos, mono::IEntityPtr entity) = 0;
        virtual void HandleMouseUp(const math::Vector2f& world_pos) = 0;
        virtual void HandleMousePosition(const math::Vector2f& world_pos) = 0;
    };
}


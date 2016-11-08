//
//  CameraTool.hpp
//  MonoiOS
//
//  Created by Niklas Damberg on 03/07/16.
//
//

#pragma once

#include "MonoPtrFwd.h"
#include "Math/Vector2f.h"

namespace editor
{
    class CameraTool
    {
    public:

        CameraTool(const mono::ICameraPtr& camera, const math::Vector2f& window_size);

        bool IsActive() const;
        void HandleMouseDown(const math::Vector2f& world_pos);
        void HandleMouseUp(const math::Vector2f& world_pos);
        void HandleMousePosition(const math::Vector2f& world_pos);

        void HandleMouseWheel(float x, float y);
        void HandleMultiGesture(const math::Vector2f& screen_position, float distance);

        mono::ICameraPtr m_camera;
        const math::Vector2f m_windowSize;

        bool m_translate;
        math::Vector2f m_translateDelta;
    };
}

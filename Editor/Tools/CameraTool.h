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
        void HandleMouseDown(const math::Vector2f& screen_position);
        void HandleMouseUp(const math::Vector2f& screen_position);
        void HandleMousePosition(const math::Vector2f& screen_position);

        void HandleMouseWheel(float x, float y);
        void HandleMultiGesture(const math::Vector2f& screen_position, float distance);

    private:
    
        mono::ICameraPtr m_camera;
        const math::Vector2f m_windowSize;

        bool m_translate;
        math::Vector2f m_translateDelta;
    };
}

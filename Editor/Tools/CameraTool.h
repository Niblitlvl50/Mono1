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

        CameraTool(const mono::ICameraPtr& camera, const math::Vector& window_size);

        bool IsActive() const;
        void HandleMouseDown(const math::Vector& screen_position);
        void HandleMouseUp(const math::Vector& screen_position);
        void HandleMousePosition(const math::Vector& screen_position);

        void HandleMouseWheel(float x, float y);
        void HandleMultiGesture(const math::Vector& screen_position, float distance);

    private:
    
        mono::ICameraPtr m_camera;
        const math::Vector m_windowSize;

        bool m_translate;
        math::Vector m_translateDelta;
    };
}

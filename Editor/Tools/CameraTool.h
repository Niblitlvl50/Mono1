
#pragma once

#include "MonoPtrFwd.h"
#include "Math/Vector.h"
#include "System/System.h"

namespace editor
{
    class CameraTool
    {
    public:

        CameraTool(const mono::ICameraPtr& camera, const System::IWindow* window);

        bool IsActive() const;
        void HandleMouseDown(const math::Vector& screen_position);
        void HandleMouseUp(const math::Vector& screen_position);
        void HandleMousePosition(const math::Vector& screen_position);

        void HandleMouseWheel(float x, float y);
        void HandleMultiGesture(const math::Vector& screen_position, float distance);

    private:
    
        mono::ICameraPtr m_camera;
        const System::IWindow* m_window;

        bool m_translate;
        math::Vector m_translateDelta;
    };
}

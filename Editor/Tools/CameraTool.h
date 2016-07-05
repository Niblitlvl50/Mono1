//
//  CameraTool.hpp
//  MonoiOS
//
//  Created by Niklas Damberg on 03/07/16.
//
//

#pragma once

#include "ITool.h"
#include "MonoPtrFwd.h"
#include "Vector2f.h"

namespace editor
{
    class CameraTool : public ITool
    {
    public:

        CameraTool(const mono::ICameraPtr& camera);

        virtual Coordinate CoordinateSystem() const;

        virtual void Begin();
        virtual void End();
        virtual bool IsActive() const;
        virtual void HandleMouseDown(const math::Vector2f& world_pos);
        virtual void HandleMouseUp(const math::Vector2f& world_pos);
        virtual void HandleMousePosition(const math::Vector2f& world_pos);

        void HandleMultiGesture();

        mono::ICameraPtr m_camera;
        const math::Vector2f m_windowSize;

        bool m_translate;
        math::Vector2f m_translateDelta;

        math::Vector2f m_startPosition;
        math::Vector2f m_cameraStartPosition;
    };
}

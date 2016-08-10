//
//  EditorCameraController.hpp
//  MonoiOS
//
//  Created by Niklas Damberg on 26/06/16.
//
//

#pragma once

#include "MonoPtrFwd.h"
#include "EventFwd.h"
#include "EventToken.h"

#include "MainMenuOptions.h"

#include "PolygonTool.h"
#include "PolygonBrushTool.h"
#include "TranslateTool.h"
#include "RotateTool.h"
#include "CameraTool.h"

namespace editor
{
    struct UIContext;
    
    class UserInputController
    {
    public:

        UserInputController(const mono::ICameraPtr& camera,
                         editor::EditorZone* editor,
                         editor::UIContext* context,
                         const math::Vector2f& window_size,
                         mono::EventHandler& event_handler);
        ~UserInputController();

        void HandleContextMenu(int item_index);
        void SelectTool(ToolsMenuOptions option);

    private:

        bool OnMouseDown(const event::MouseDownEvent& event);
        bool OnMouseUp(const event::MouseUpEvent& event);
        bool OnMouseMove(const event::MouseMotionEvent& event);
        bool OnMultiGesture(const event::MultiGestureEvent& event);
        bool OnKeyDown(const event::KeyDownEvent& event);

        mono::EventHandler& m_eventHandler;

        mono::EventToken<event::MouseUpEvent> m_mouseUpToken;
        mono::EventToken<event::MouseDownEvent> m_mouseDownToken;
        mono::EventToken<event::MouseMotionEvent> m_mouseMoveToken;
        mono::EventToken<event::MultiGestureEvent> m_multiGestureToken;
        mono::EventToken<event::KeyDownEvent> m_keyDownToken;

        editor::UIContext* m_context;

        editor::CameraTool m_cameraTool;
        editor::PolygonTool m_polygonTool;
        editor::PolygonBrushTool m_polygonBrushTool;
        editor::TranslateTool m_translateTool;
        editor::RotateTool m_rotateTool;
        editor::ITool* m_activeTool;
    };
}


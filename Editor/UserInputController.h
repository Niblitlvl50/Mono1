
#pragma once

#include "MonoPtrFwd.h"
#include "Events/EventFwd.h"
#include "EventHandler/EventToken.h"

#include "MainMenuOptions.h"

#include "Tools/PolygonTool.h"
#include "Tools/PolygonBrushTool.h"
#include "Tools/TranslateTool.h"
#include "Tools/RotateTool.h"
#include "Tools/CameraTool.h"
#include "Tools/PathTool.h"

#include "System/System.h"

namespace editor
{
    struct UIContext;
    struct Grabber;
    
    class UserInputController
    {
    public:

        UserInputController(const mono::ICameraPtr& camera,
                            System::IWindow* window,
                            editor::Editor* editor,
                            editor::UIContext* context,
                            mono::EventHandler& event_handler);
        ~UserInputController();

        void HandleContextMenu(int item_index);
        void SelectTool(ToolsMenuOptions option);

    private:

        bool OnMouseDown(const event::MouseDownEvent& event);
        bool OnMouseUp(const event::MouseUpEvent& event);
        bool OnMouseMove(const event::MouseMotionEvent& event);
        bool OnMouseWheel(const event::MouseWheelEvent& event);
        bool OnMultiGesture(const event::MultiGestureEvent& event);
        bool OnKeyDown(const event::KeyDownEvent& event);

        System::IWindow* m_window;
        mono::EventHandler& m_eventHandler;
        editor::Editor* m_editor;

        mono::EventToken<event::MouseUpEvent> m_mouseUpToken;
        mono::EventToken<event::MouseDownEvent> m_mouseDownToken;
        mono::EventToken<event::MouseMotionEvent> m_mouseMoveToken;
        mono::EventToken<event::MouseWheelEvent> m_mouseWheelToken;
        mono::EventToken<event::MultiGestureEvent> m_multiGestureToken;
        mono::EventToken<event::KeyDownEvent> m_keyDownToken;

        editor::UIContext* m_context;

        editor::CameraTool m_cameraTool;
        editor::TranslateTool m_translateTool;
        editor::RotateTool m_rotateTool;
        editor::PolygonTool m_polygonTool;
        editor::PolygonBrushTool m_polygonBrushTool;
        editor::PathTool m_pathTool;
        editor::ITool* m_activeTool;

        editor::Grabber* m_grabber;

        bool m_isMaximized;
    };
}


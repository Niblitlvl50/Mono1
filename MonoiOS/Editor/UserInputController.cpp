//
//  EditorCameraController.cpp
//  MonoiOS
//
//  Created by Niklas Damberg on 26/06/16.
//
//

#include "UserInputController.h"

#include "EventHandler.h"
#include "EventFuncFwd.h"
#include "MouseDownEvent.h"
#include "MouseUpEvent.h"
#include "MouseMotionEvent.h"
#include "MultiGestureEvent.h"
#include "KeyDownEvent.h"

#include "SysKeycodes.h"

#include "ImGuiInterfaceDrawer.h"


using namespace editor;

UserInputController::UserInputController(const mono::ICameraPtr& camera,
                                         editor::EditorZone* editor,
                                         editor::UIContext* context,
                                         const math::Vector2f& window_size,
                                         mono::EventHandler& event_handler)
    : m_eventHandler(event_handler),
      m_context(context),
      m_cameraTool(camera, window_size),
      m_polygonTool(editor),
      m_polygonBrushTool(editor),
      m_translateTool(editor),
      m_rotateTool(editor),
      m_activeTool(&m_polygonTool)
{
    using namespace std::placeholders;

    const event::MouseDownEventFunc& mouse_down = std::bind(&UserInputController::OnMouseDown, this, _1);
    const event::MouseUpEventFunc& mouse_up = std::bind(&UserInputController::OnMouseUp, this, _1);
    const event::MouseMotionEventFunc& mouse_move = std::bind(&UserInputController::OnMouseMove, this, _1);
    const event::MultiGestureEventFunc& multi_gesture = std::bind(&UserInputController::OnMultiGesture, this, _1);
    const event::KeyDownEventFunc& key_down = std::bind(&UserInputController::OnKeyDown, this, _1);

    m_mouseDownToken = m_eventHandler.AddListener(mouse_down);
    m_mouseUpToken = m_eventHandler.AddListener(mouse_up);
    m_mouseMoveToken = m_eventHandler.AddListener(mouse_move);
    m_multiGestureToken = m_eventHandler.AddListener(multi_gesture);
    m_keyDownToken = m_eventHandler.AddListener(key_down);
}

UserInputController::~UserInputController()
{
    m_eventHandler.RemoveListener(m_mouseDownToken);
    m_eventHandler.RemoveListener(m_mouseUpToken);
    m_eventHandler.RemoveListener(m_mouseMoveToken);
    m_eventHandler.RemoveListener(m_multiGestureToken);
    m_eventHandler.RemoveListener(m_keyDownToken);
}

void UserInputController::HandleContextMenu(int item_index)
{
    if(item_index == 0)
        m_activeTool->IsActive() ? m_activeTool->End() : m_activeTool->Begin();
}

void UserInputController::SelectTool(ToolsMenuOptions option)
{
    switch(option)
    {
        case ToolsMenuOptions::POLYGON_TOOL:
        {
            m_activeTool = &m_polygonTool;
            m_context->notifications.push_back(Notification(2, "Polygon tool", 2000));
            break;
        }

        case ToolsMenuOptions::POLYGON_BRUSH_TOOL:
        {
            m_activeTool = &m_polygonBrushTool;
            m_context->notifications.push_back(Notification(2, "Polygon Brush", 2000));
            break;
        }

        case ToolsMenuOptions::TRANSLATE_TOOL:
        {
            m_activeTool = &m_translateTool;
            m_context->notifications.push_back(Notification(2, "Translate tool", 2000));
            break;
        }

        case ToolsMenuOptions::ROTATE_TOOL:
        {
            m_activeTool = &m_rotateTool;
            m_context->notifications.push_back(Notification(2, "Rotate tool", 2000));
            break;
        }

        case ToolsMenuOptions::CAMERA_TOOL:
            break;
    }
}

bool UserInputController::OnMouseDown(const event::MouseDownEvent& event)
{
    bool handled = false;

    if(event.key == MouseButton::LEFT)
    {
        m_activeTool->HandleMouseDown(math::Vector2f(event.worldX, event.worldY));
        handled = m_activeTool->IsActive();
    }

    if(!handled)
        m_cameraTool.HandleMouseDown(math::Vector2f(event.screenX, event.screenY));

    return true;
}

bool UserInputController::OnMouseUp(const event::MouseUpEvent& event)
{
    if(event.key == MouseButton::LEFT)
        m_activeTool->HandleMouseUp(math::Vector2f(event.worldX, event.worldY));
    else if(event.key == MouseButton::RIGHT)
        m_context->showContextMenu = true;

    m_cameraTool.HandleMouseUp(math::Vector2f(event.screenX, event.screenY));

    return true;
}

bool UserInputController::OnMouseMove(const event::MouseMotionEvent& event)
{
    m_activeTool->HandleMousePosition(math::Vector2f(event.worldX, event.worldY));
    m_cameraTool.HandleMousePosition(math::Vector2f(event.screenX, event.screenY));

    return true;
}

bool UserInputController::OnMultiGesture(const event::MultiGestureEvent& event)
{
    m_cameraTool.HandleMultiGesture(math::Vector2f(event.x, event.y), event.distance);
    return true;
}

bool UserInputController::OnKeyDown(const event::KeyDownEvent& event)
{
    if(event.key == Key::ONE)
        SelectTool(ToolsMenuOptions::POLYGON_TOOL);
    else if(event.key == Key::TWO)
        SelectTool(ToolsMenuOptions::POLYGON_BRUSH_TOOL);
    else if(event.key == Key::THREE)
        SelectTool(ToolsMenuOptions::TRANSLATE_TOOL);
    else if(event.key == Key::FOUR)
        SelectTool(ToolsMenuOptions::ROTATE_TOOL);

    return false;
}


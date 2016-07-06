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
      m_translateTool(editor),
      m_rotateTool(editor),
      m_activeTool(&m_polygonTool)
{
    using namespace std::placeholders;

    const event::MouseDownEventFunc& mouse_down = std::bind(&UserInputController::OnMouseDown, this, _1);
    const event::MouseUpEventFunc& mouse_up = std::bind(&UserInputController::OnMouseUp, this, _1);
    const event::MouseMotionEventFunc& mouse_move = std::bind(&UserInputController::OnMouseMove, this, _1);
    const event::MultiGestureEventFunc& multi_gesture = std::bind(&UserInputController::OnMultiGesture, this, _1);

    m_mouseDownToken = m_eventHandler.AddListener(mouse_down);
    m_mouseUpToken = m_eventHandler.AddListener(mouse_up);
    m_mouseMoveToken = m_eventHandler.AddListener(mouse_move);
    m_multiGestureToken = m_eventHandler.AddListener(multi_gesture);
}

UserInputController::~UserInputController()
{
    m_eventHandler.RemoveListener(m_mouseDownToken);
    m_eventHandler.RemoveListener(m_mouseUpToken);
    m_eventHandler.RemoveListener(m_mouseMoveToken);
    m_eventHandler.RemoveListener(m_multiGestureToken);
}

void UserInputController::HandleContextMenu(int item_index)
{
    if(item_index == 0)
        m_activeTool->IsActive() ? m_activeTool->End() : m_activeTool->Begin();
}

void UserInputController::SelectTool(int tool_index)
{
    if(tool_index == 0)
    {
        m_activeTool = &m_polygonTool;
        m_context->notifications.push_back(Notification(1, "Polygon tool", 2000));
    }
    else if(tool_index == 1)
    {
        m_activeTool = &m_translateTool;
        m_context->notifications.push_back(Notification(1, "Translate tool", 2000));
    }
    else if(tool_index == 2)
    {
        m_activeTool = &m_rotateTool;
        m_context->notifications.push_back(Notification(1, "Rotate tool", 2000));
    }
    //else if(tool_index == 2)
    //    m_activeTool = &m_cameraTool;
}

bool UserInputController::OnMouseDown(const event::MouseDownEvent& event)
{
    if(event.key == MouseButton::LEFT)
    {
        m_activeTool->HandleMouseDown(math::Vector2f(event.worldX, event.worldY));
        const bool tool_active = m_activeTool->IsActive();
        if(!tool_active)
            m_cameraTool.HandleMouseDown(math::Vector2f(event.screenX, event.screenY));

        return true;
    }

    return false;
}

bool UserInputController::OnMouseUp(const event::MouseUpEvent& event)
{
    if(event.key == MouseButton::LEFT)
    {
        m_activeTool->HandleMouseUp(math::Vector2f(event.worldX, event.worldY));
        const bool tool_active = m_activeTool->IsActive();
        if(!tool_active)
            m_cameraTool.HandleMouseUp(math::Vector2f(event.screenX, event.screenY));
    }
    else if(event.key == MouseButton::RIGHT)
    {
        m_context->showContextMenu = true;
    }

    return true;
}

bool UserInputController::OnMouseMove(const event::MouseMotionEvent& event)
{
    m_activeTool->HandleMousePosition(math::Vector2f(event.worldX, event.worldY));
    const bool tool_active = m_activeTool->IsActive();
    if(!tool_active)
        m_cameraTool.HandleMousePosition(math::Vector2f(event.screenX, event.screenY));

    return true;
}

bool UserInputController::OnMultiGesture(const event::MultiGestureEvent& event)
{
    m_cameraTool.HandleMultiGesture(math::Vector2f(event.x, event.y), event.distance);
    return true;
}

//
//  EditorCameraController.cpp
//  MonoiOS
//
//  Created by Niklas Damberg on 26/06/16.
//
//

#include "UserInputController.h"

#include "Editor.h"
#include "IWindow.h"
#include "EventHandler/EventHandler.h"
#include "Events/EventFuncFwd.h"
#include "Events/MouseEvent.h"
#include "Events/MultiGestureEvent.h"
#include "Events/KeyEvent.h"
#include "System/SysKeycodes.h"

#include "ImGuiInterfaceDrawer.h"


namespace
{
    struct ToolData
    {
        editor::ITool* tool;
        editor::Notification notification;
        std::vector<std::string> context_menu;
    };

    ToolData tools[editor::ToolsMenuOptions::N_TOOLS];
}

using namespace editor;

UserInputController::UserInputController(const mono::ICameraPtr& camera,
                                         const mono::IWindowPtr& window,
                                         editor::Editor* editor,
                                         editor::UIContext* context,
                                         mono::EventHandler& event_handler)
    : m_window(window),
      m_eventHandler(event_handler),
      m_editor(editor),
      m_context(context),
      m_cameraTool(camera, window->Size()),
      m_polygonTool(editor),
      m_polygonBrushTool(editor),
      m_pathTool(editor),
      m_activeTool(nullptr),
      m_isMaximized(false)
{
    using namespace std::placeholders;

    const event::MouseDownEventFunc& mouse_down = std::bind(&UserInputController::OnMouseDown, this, _1);
    const event::MouseUpEventFunc& mouse_up = std::bind(&UserInputController::OnMouseUp, this, _1);
    const event::MouseMotionEventFunc& mouse_move = std::bind(&UserInputController::OnMouseMove, this, _1);
    const event::MouseWheelEventFunc& mouse_wheel = std::bind(&UserInputController::OnMouseWheel, this, _1);
    const event::MultiGestureEventFunc& multi_gesture = std::bind(&UserInputController::OnMultiGesture, this, _1);
    const event::KeyDownEventFunc& key_down = std::bind(&UserInputController::OnKeyDown, this, _1);

    m_mouseDownToken = m_eventHandler.AddListener(mouse_down);
    m_mouseUpToken = m_eventHandler.AddListener(mouse_up);
    m_mouseMoveToken = m_eventHandler.AddListener(mouse_move);
    m_mouseWheelToken = m_eventHandler.AddListener(mouse_wheel);
    m_multiGestureToken = m_eventHandler.AddListener(multi_gesture);
    m_keyDownToken = m_eventHandler.AddListener(key_down);

    tools[ToolsMenuOptions::POLYGON_TOOL] = {
        &m_polygonTool,
        Notification(m_context->default_icon, "Polygon tool", 2000),
        { "Create polygon", "Undo last" }
    };

    tools[ToolsMenuOptions::POLYGON_BRUSH_TOOL] = {
        &m_polygonBrushTool,
        Notification(m_context->default_icon, "Polygon Brush", 2000),
        { }
    };

    tools[ToolsMenuOptions::TRANSLATE_TOOL] = {
        &m_translateTool,
        Notification(m_context->default_icon, "Translate tool", 2000),
        { }
    };

    tools[ToolsMenuOptions::ROTATE_TOOL] = {
        &m_rotateTool,
        Notification(m_context->default_icon, "Rotate tool", 2000),
        { }
    };

    tools[ToolsMenuOptions::PATH_TOOL] = {
        &m_pathTool,
        Notification(m_context->default_icon, "Path tool", 2000),
        { "Create path", "Undo last" }
    };

    SelectTool(ToolsMenuOptions::TRANSLATE_TOOL);
}

UserInputController::~UserInputController()
{
    m_eventHandler.RemoveListener(m_mouseDownToken);
    m_eventHandler.RemoveListener(m_mouseUpToken);
    m_eventHandler.RemoveListener(m_mouseMoveToken);
    m_eventHandler.RemoveListener(m_mouseWheelToken);
    m_eventHandler.RemoveListener(m_multiGestureToken);
    m_eventHandler.RemoveListener(m_keyDownToken);
}

void UserInputController::HandleContextMenu(int item_index)
{
    m_activeTool->HandleContextMenu(item_index);
}

void UserInputController::SelectTool(ToolsMenuOptions option)
{
    const ToolData& tool_data = tools[option];

    m_activeTool = tool_data.tool;
    m_context->notifications.push_back(tool_data.notification);
    m_context->contextMenuItems = tool_data.context_menu;
    m_context->active_tool_index = static_cast<int>(option);

    m_activeTool->Begin();
}

bool UserInputController::OnMouseDown(const event::MouseDownEvent& event)
{
    bool handled = false;

    if(event.key == MouseButton::LEFT)
    {
        const math::Vector2f world_position(event.worldX, event.worldY);

        auto entity = m_editor->FindEntityFromPosition(world_position);
        m_editor->SelectEntity(entity);
        m_activeTool->HandleMouseDown(world_position, entity);
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
        m_context->showContextMenu = !m_context->contextMenuItems.empty();

    m_cameraTool.HandleMouseUp(math::Vector2f(event.screenX, event.screenY));

    return true;
}

bool UserInputController::OnMouseMove(const event::MouseMotionEvent& event)
{
    if(m_activeTool->IsActive())
        m_activeTool->HandleMousePosition(math::Vector2f(event.worldX, event.worldY));

    if(m_cameraTool.IsActive())
        m_cameraTool.HandleMousePosition(math::Vector2f(event.screenX, event.screenY));

    return true;
}

bool UserInputController::OnMouseWheel(const event::MouseWheelEvent& event)
{
    m_cameraTool.HandleMouseWheel(event.x, event.y);
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
        SelectTool(ToolsMenuOptions::TRANSLATE_TOOL);
    else if(event.key == Key::TWO)
        SelectTool(ToolsMenuOptions::ROTATE_TOOL);
    else if(event.key == Key::THREE)
        SelectTool(ToolsMenuOptions::POLYGON_TOOL);
    else if(event.key == Key::FOUR)
        SelectTool(ToolsMenuOptions::POLYGON_BRUSH_TOOL);
    else if(event.key == Key::FIVE)
        SelectTool(ToolsMenuOptions::PATH_TOOL);
    else if(event.key == Key::ENTER && event.ctrl)
    {
        if(m_isMaximized)
            m_window->RestoreSize();
        else
            m_window->Maximize();

        m_isMaximized = !m_isMaximized;
    }

    return false;
}


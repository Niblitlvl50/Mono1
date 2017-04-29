
#pragma once

#include "MainMenuOptions.h"
#include "Math/Quad.h"
#include "UINotification.h"

#include <functional>
#include <vector>
#include <string>

namespace editor
{
    enum UIComponent
    {
        NONE = 1,
        NAME = 2,
        NAME_EDITABLE = 4,
        POSITIONAL = 8,
        TEXTURAL = 16
    };

    struct UIEntityItem
    {
        int texture_id = 0;
        math::Quad icon;
        std::string tooltip;
    };

    struct UIContext
    {
        int tools_texture_id = 0;
        math::Quad default_icon;

        int active_tool_index = 0;

        const char** texture_items = nullptr;
        int texture_items_count = 0;

        unsigned int components = UIComponent::NONE;
        const char* name = nullptr;
        math::Vector position;
        float rotation = 0.0f;
        int texture_index = 0;

        std::string drag_context;
        bool showContextMenu = false;

        std::vector<std::string> contextMenuItems;
        std::vector<UIEntityItem> entity_items;
        std::vector<Notification> notifications;

        std::function<void (int)> texture_changed_callback;
        std::function<void (const char*)> name_callback;
        std::function<void ()> delete_callback;
        std::function<void (int)> context_menu_callback;
        std::function<void (EditorMenuOptions option)> editor_menu_callback;
        std::function<void (ToolsMenuOptions option)> tools_menu_callback;
        std::function<void (const std::string& id, const math::Vector& position)> drop_callback;
    };
}

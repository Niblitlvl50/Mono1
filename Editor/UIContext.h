
#pragma once

#include "MainMenuOptions.h"
#include "Math/Quad.h"

#include <functional>
#include <vector>
#include <string>

namespace editor
{
    struct Notification
    {
        Notification()
        { }

        Notification(const math::Quad& icon, const std::string& text, int time_left)
            : icon(icon),
              text(text),
              time_left(time_left)
        { }

        math::Quad icon;
        std::string text;
        int time_left;
    };

    struct UIContext
    {
        int tools_texture_id;
        math::Quad default_icon;

        bool showContextMenu;
        std::vector<std::string> contextMenuItems;

        const char** texture_items;
        int texture_items_count;

        bool has_selection;
        float polygon_x;
        float polygon_y;
        float polygon_rotation;
        float texture_repeate;
        int texture_index;

        int active_tool_index;

        std::function<void (float)> texture_repeate_callback;
        std::function<void (int)> texture_changed_callback;
        std::function<void ()> delete_callback;
        std::function<void (int)> contextMenuCallback;
        std::function<void (EditorMenuOptions option)> editorMenuCallback;
        std::function<void (ToolsMenuOptions option)> toolsMenuCallback;

        std::vector<Notification> notifications;
    };
}

//
//  ImGuiInterfaceDrawer.hpp
//  MonoiOS
//
//  Created by Niklas Damberg on 20/06/16.
//
//

#pragma once

#include "IUpdatable.h"
#include "MainMenuOptions.h"

#include <functional>
#include <vector>
#include <string>

namespace editor
{
    struct Notification
    {
        Notification()
        { }

        Notification(int image, const std::string& text, int time_left)
            : image(image),
              text(text),
              time_left(time_left)
        { }

        int image;
        std::string text;
        int time_left;
    };

    struct UIContext
    {
        bool showContextMenu;
        std::vector<std::string> contextMenuItems;
        std::function<void (int)> contextMenuCallback;

        const char** texture_items;
        int texture_items_count;

        bool has_selection;
        float polygon_x;
        float polygon_y;
        float polygon_rotation;
        float texture_repeate;
        int texture_index;
        std::function<void (float)> texture_repeate_callback;
        std::function<void (int)> texture_changed_callback;
        std::function<void ()> delete_callback;

        std::vector<Notification> notifications;

        std::function<void (EditorMenuOptions option)> editorMenuCallback;
        std::function<void (ToolsMenuOptions option)> toolsMenuCallback;
    };

    class ImGuiInterfaceDrawer : public mono::IUpdatable
    {
    public:

        ImGuiInterfaceDrawer(UIContext& context);

        virtual void doUpdate(unsigned int delta);

        UIContext& m_context;
    };
}


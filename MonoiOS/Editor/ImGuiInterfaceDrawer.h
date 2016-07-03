//
//  ImGuiInterfaceDrawer.hpp
//  MonoiOS
//
//  Created by Niklas Damberg on 20/06/16.
//
//

#pragma once

#include "IUpdatable.h"

#include <functional>
#include <vector>
#include <string>

namespace editor
{
    struct Notification
    {
        int image;
        std::string text;
    };

    struct UIContext
    {
        bool showContextMenu;
        std::vector<std::string> contextMenuItems;
        std::function<void (int)> contextMenuCallback;

        int selectedPolygonIndex;
        std::vector<std::string> polygonItems;
        std::function<void (int)> polygonSelected;
        std::function<void (int)> polygonDeleted;

        std::vector<Notification> notifications;

        std::function<void (int)> editorMenuCallback;
        std::function<void (int)> toolsMenuCallback;
    };

    class ImGuiInterfaceDrawer : public mono::IUpdatable
    {
    public:

        ImGuiInterfaceDrawer(UIContext& context);

        virtual void doUpdate(unsigned int delta);

        UIContext& m_context;
    };
}


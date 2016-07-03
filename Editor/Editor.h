//
//  Editor.hpp
//  MonoiOS
//
//  Created by Niklas Damberg on 02/07/16.
//
//

#pragma once

#include "ZoneBase.h"
#include "EventFwd.h"

#include "ImGuiInterfaceDrawer.h"
#include "ImGuiInputHandler.h"

#include "Polygon.h"


namespace editor
{
    class UserInputController;

    class EditorZone : public mono::ZoneBase
    {
    public:

        EditorZone(const math::Vector2f& window_size, mono::EventHandler& event_handler);
        virtual ~EditorZone();

        virtual void OnLoad(mono::ICameraPtr camera);
        virtual void OnUnload();

        void AddPolygon(const std::shared_ptr<editor::PolygonEntity>& polygon);

        void OnContextMenu(int index);
        void OnSelectedPolygon(int index);
        void OnDeletePolygon(int index);

        void EditorMenuCallback(int index);
        void ToolsMenuCallback(int index);

        const math::Vector2f m_windowSize;
        mono::EventHandler& m_eventHandler;
        editor::ImGuiInputHandler m_inputHandler;

        editor::UIContext m_context;
        std::shared_ptr<editor::ImGuiInterfaceDrawer> m_interfaceDrawer;
        std::shared_ptr<editor::UserInputController> m_userInputController;
        std::vector<std::shared_ptr<editor::PolygonEntity>> m_polygons;
    };
}

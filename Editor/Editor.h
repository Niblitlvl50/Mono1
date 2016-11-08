//
//  Editor.hpp
//  MonoiOS
//
//  Created by Niklas Damberg on 02/07/16.
//
//

#pragma once

#include "ZoneBase.h"
#include "Events/EventFwd.h"
#include "EventHandler/EventToken.h"

#include "MainMenuOptions.h"
#include "UIContext.h"
#include "ImGuiInterfaceDrawer.h"
#include "ImGuiImpl/ImGuiInputHandler.h"

#include "Polygon.h"

class ImGuiRenderer;

namespace editor
{
    class UserInputController;

    class Editor : public mono::ZoneBase
    {
    public:

        Editor(const mono::IWindowPtr& window, mono::EventHandler& event_handler, const char* file_name);
        virtual ~Editor();

        virtual void OnLoad(mono::ICameraPtr camera);
        virtual void OnUnload();

        bool OnSurfaceChanged(const event::SurfaceChangedEvent& event);

        void AddPolygon(const std::shared_ptr<editor::PolygonEntity>& polygon);
        void AddPath(const std::vector<math::Vector2f>& points);

        mono::IEntityPtr FindEntityFromPosition(const math::Vector2f& position);
        void SelectEntity(const mono::IEntityPtr& entity);

        void OnContextMenu(int index);
        void OnTextureRepeate(float repeate);
        void OnTextureChanged(int texture_index);
        void OnDeletePolygon();

        void EditorMenuCallback(EditorMenuOptions index);
        void ToolsMenuCallback(ToolsMenuOptions index);

    private:

        mono::IWindowPtr m_window;
        mono::EventHandler& m_eventHandler;
        ImGuiInputHandler m_inputHandler;
        const char* m_fileName;
        mono::ICameraPtr m_camera;

        editor::UIContext m_context;
        std::shared_ptr<editor::ImGuiInterfaceDrawer> m_interfaceDrawer;
        std::shared_ptr<ImGuiRenderer> m_guiRenderer;
        std::shared_ptr<editor::UserInputController> m_userInputController;

        std::shared_ptr<editor::PolygonEntity> m_selected_polygon;
        std::vector<std::shared_ptr<editor::PolygonEntity>> m_polygons;

        mono::EventToken<event::SurfaceChangedEvent> m_surfaceChangedToken;
    };
}

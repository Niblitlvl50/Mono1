
#pragma once

#include "ZoneBase.h"
#include "Events/EventFwd.h"
#include "EventHandler/EventToken.h"

#include "MainMenuOptions.h"
#include "UIContext.h"
#include "ImGuiImpl/ImGuiInputHandler.h"

#include "IObjectProxy.h"
#include "Polygon.h"
#include "Path.h"
#include "SpriteEntity.h"
#include "Grabber.h"
#include "SnapPoint.h"
#include "Prefab.h"

#include "EntityRepository.h"


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

        void Load();
        void Save();

        bool OnSurfaceChanged(const event::SurfaceChangedEvent& event);

        void AddPolygon(const std::shared_ptr<editor::PolygonEntity>& polygon);
        void AddPath(const std::shared_ptr<editor::PathEntity>& path);
        void AddObject(const std::shared_ptr<editor::SpriteEntity>& object);
        void AddPrefab(const std::shared_ptr<editor::Prefab>& prefab);

        void SelectProxyObject(IObjectProxy* proxy_object);
        IObjectProxy* FindProxyObject(const math::Vector& position);

        void SelectGrabber(const math::Vector& position);
        Grabber* FindGrabber(const math::Vector& position);
        void UpdateGrabbers();

        std::pair<int, math::Vector> FindSnapPosition(const math::Vector& position) const;

        float GetPickingDistance() const;

        void OnDeleteObject();
        void OnContextMenu(int index);
        void EditorMenuCallback(EditorMenuOptions index);
        void ToolsMenuCallback(ToolsMenuOptions index);
        void DropItemCallback(const std::string& id, const math::Vector& position);

    //private:

        mono::IWindowPtr m_window;
        mono::EventHandler& m_eventHandler;
        ImGuiInputHandler m_inputHandler;
        const char* m_fileName;
        mono::ICameraPtr m_camera;

        editor::UIContext m_context;
        EntityRepository m_entityRepository;

        std::shared_ptr<ImGuiRenderer> m_guiRenderer;
        std::shared_ptr<editor::UserInputController> m_userInputController;

        std::vector<editor::Grabber> m_grabbers;
        std::vector<editor::SnapPoint> m_snap_points;

        unsigned int m_seleced_id;
        std::vector<std::unique_ptr<IObjectProxy>> m_object_proxies;
        std::vector<std::shared_ptr<editor::PolygonEntity>> m_polygons;
        std::vector<std::shared_ptr<editor::PathEntity>> m_paths;
        std::vector<std::shared_ptr<editor::SpriteEntity>> m_objects;
        std::vector<std::shared_ptr<editor::Prefab>> m_prefabs;

        mono::EventToken<event::SurfaceChangedEvent> m_surfaceChangedToken;
    };
}

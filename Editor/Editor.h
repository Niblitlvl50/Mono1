//
//  Editor.hpp
//  MonoiOS
//
//  Created by Niklas Damberg on 02/07/16.
//
//

#pragma once

#include "ZoneBase.h"

#include "Polygon.h"
#include "PolygonTool.h"
#include "RotateTool.h"

#include "EventFwd.h"

#include "ImGuiInterfaceDrawer.h"
#include "ImGuiInputHandler.h"


namespace editor
{
    class CameraController;

    class EditorZone : public mono::ZoneBase
    {
    public:

        EditorZone(const math::Vector2f& window_size, mono::EventHandler& event_handler);
        virtual ~EditorZone();

        virtual void OnLoad(mono::ICameraPtr camera);
        virtual void OnUnload();

        void AddPolygon(const std::shared_ptr<editor::PolygonEntity>& polygon);

        bool OnMouseDown(const event::MouseDownEvent& event);
        bool OnMouseUp(const event::MouseUpEvent& event);
        bool OnMouseMove(const event::MouseMotionEvent& event);
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
        
        mono::EventToken<event::MouseDownEvent> m_mouseDownToken;
        mono::EventToken<event::MouseUpEvent> m_mouseUpToken;
        mono::EventToken<event::MouseMotionEvent> m_mouseMoveToken;
        
        mono::ICameraPtr m_camera;
        std::shared_ptr<editor::CameraController> m_cameraController;
        
        std::vector<std::shared_ptr<editor::PolygonEntity>> m_polygons;

        editor::PolygonTool m_polygonTool;
        editor::RotateTool m_rotateTool;
        editor::ITool* m_activeTool;
    };
}

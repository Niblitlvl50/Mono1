

#include "SysLibs.h"
#include "SysKeycodes.h"

#include "WindowFactory.h"
#include "TraceCamera.h"
#include "Engine.h"

#include "EventHandler.h"
#include "EventToken.h"
#include "EventFwd.h"
#include "EventFuncFwd.h"
#include "MouseUpEvent.h"
#include "MouseDownEvent.h"
#include "MouseMotionEvent.h"

#include "IWindow.h"
#include "ICamera.h"
#include "IZone.h"
#include "IRenderer.h"

#include "ZoneBase.h"
#include "TextFunctions.h"
#include "Color.h"
#include "Sprite.h"
#include "EntityBase.h"
#include "CameraController.h"

#include "ImGuiInputHandler.h"
#include "ImGuiInterfaceDrawer.h"
#include "ImGuiRenderer.h"

#include "Polygon.h"

namespace
{
    class SpriteDrawable : public mono::EntityBase
    {
    public:

        SpriteDrawable(const char* file)
        : m_sprite(file)
        {
            mPosition = math::Vector2f(600, 400);
            mScale = math::Vector2f(50, 50);
        }

        virtual void Draw(mono::IRenderer& renderer) const
        {
            renderer.DrawSprite(m_sprite);
        }

        virtual void Update(unsigned int delta)
        {
            m_sprite.doUpdate(delta);
        }
        
        mono::Sprite m_sprite;
    };

    class EditorZone : public mono::ZoneBase
    {
    public:

        EditorZone(const math::Vector2f& window_size, mono::EventHandler& event_handler)
            : m_windowSize(window_size),
              m_eventHandler(event_handler),
              m_inputHandler(event_handler)
        {
            using namespace std::placeholders;

            const event::MouseUpEventFunc& mouse_up = std::bind(&EditorZone::OnMouseUp, this, _1);
            const event::MouseDownEventFunc& mouse_down = std::bind(&EditorZone::OnMouseDown, this, _1);
            const event::MouseMotionEventFunc& mouse_move = std::bind(&EditorZone::OnMouseMove, this, _1);

            m_mouseUpToken = m_eventHandler.AddListener(mouse_up);
            m_mouseDownToken = m_eventHandler.AddListener(mouse_down);
            m_mouseMoveToken = m_eventHandler.AddListener(mouse_move);

            m_polygonIndex = -1;

            m_context.showContextMenu = false;
            m_context.contextMenuCallback = std::bind(&EditorZone::OnContextMenu, this, _1);
            m_context.contextMenuItems = { "Polygon", "Decoration" };

            m_context.selectedPolygonIndex = -1;
            m_context.polygonItems = { "Poly 1", "Poly 2", "Poly 3" };
            m_context.polygonCallback = std::bind(&EditorZone::OnSelectedPolygon, this, _1);

            m_interfaceDrawer = std::make_shared<editor::ImGuiInterfaceDrawer>(m_context);

            m_translate = false;
        }

        virtual ~EditorZone()
        {
            m_eventHandler.RemoveListener(m_mouseUpToken);
            m_eventHandler.RemoveListener(m_mouseDownToken);
            m_eventHandler.RemoveListener(m_mouseMoveToken);
        }

        virtual void OnLoad(mono::ICameraPtr camera)
        {
            m_camera = camera;
            m_cameraController = std::make_shared<mono::CameraController>(m_camera.get(), m_eventHandler);

            AddUpdatable(m_interfaceDrawer);
            AddDrawable(std::make_shared<editor::ImGuiRenderer>(m_windowSize), 1);
            AddDrawable(std::make_shared<SpriteDrawable>("shuttle.sprite"), 0);
        }

        virtual void OnUnload()
        { }

        bool OnMouseDown(const event::MouseDownEvent& event)
        {
            m_translate = true;
            m_translateDelta.x = event.screenX;
            m_translateDelta.y = m_windowSize.y - event.screenY;

            return false;
        }

        bool OnMouseUp(const event::MouseUpEvent& event)
        {
            m_translate = false;

            if(event.key != MouseButton::LEFT && event.key != MouseButton::RIGHT)
                return false;

            if(event.key == MouseButton::LEFT)
            {
                if(m_polygonIndex == -1)
                    return false;

                auto polygon = m_polygons.back();
                polygon->m_points.push_back(math::Vector2f(event.worldX, event.worldY));
            }
            else if(event.key == MouseButton::RIGHT)
            {
                m_context.showContextMenu = true;
            }

            return true;
        }

        bool OnMouseMove(const event::MouseMotionEvent& event)
        {
            if(!m_translate)
                return false;

            const math::Vector2f move(event.screenX, m_windowSize.y - event.screenY);
            const math::Vector2f& delta = move - m_translateDelta;

            const math::Quad& viewport = m_camera->GetViewport();
            const math::Vector2f& scale = viewport.mB / m_windowSize;

            const math::Vector2f& cam_pos = m_camera->GetPosition();
            const math::Vector2f& new_pos = cam_pos - (delta * scale);

            m_camera->SetPosition(new_pos);

            m_translateDelta = move;

            return false;
        }

        void OnContextMenu(int index)
        {
            if(index == 0)
            {
                if(m_polygonIndex == -1)
                {
                    m_polygons.push_back(std::make_shared<editor::PolygonEntity>());
                    m_polygonIndex = m_polygons.size();

                    AddEntity(m_polygons.back(), 0);
                }
                else
                {
                    m_polygonIndex = -1;
                }
            }
        }

        void OnSelectedPolygon(int index)
        {
            m_context.selectedPolygonIndex = index;
        }

        const math::Vector2f m_windowSize;
        mono::EventHandler& m_eventHandler;
        editor::ImGuiInputHandler m_inputHandler;

        editor::UIContext m_context;
        std::shared_ptr<editor::ImGuiInterfaceDrawer> m_interfaceDrawer;

        mono::EventToken<event::MouseUpEvent> m_mouseUpToken;
        mono::EventToken<event::MouseDownEvent> m_mouseDownToken;
        mono::EventToken<event::MouseMotionEvent> m_mouseMoveToken;

        mono::ICameraPtr m_camera;
        std::shared_ptr<mono::CameraController> m_cameraController;

        size_t m_polygonIndex;
        std::vector<std::shared_ptr<editor::PolygonEntity>> m_polygons;

        bool m_translate;
        math::Vector2f m_translateDelta;
    };
}

int main()
{
    constexpr math::Vector2f window_size(1200.0f, 700.0f);

    System::Init();

    // The "global" event handler used throughout the game
    mono::EventHandler eventHandler;

    mono::IWindowPtr window = mono::CreateWindow("Editor", window_size.x, window_size.y, false);
    window->SetBackgroundColor(mono::Color::RGBA(0.7, 0.7, 0.7));

    mono::ICameraPtr camera = std::make_shared<mono::TraceCamera>(window_size.x, window_size.y);
    //mono::LoadFont("pixelette.ttf", 10.0f);

    mono::Engine engine(window, camera, eventHandler);
    engine.Run(std::make_shared<EditorZone>(window_size, eventHandler));

    System::Exit();

    return 0;
}



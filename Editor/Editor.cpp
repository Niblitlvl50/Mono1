//
//  Editor.cpp
//  MonoiOS
//
//  Created by Niklas Damberg on 02/07/16.
//
//

#include "Editor.h"
#include "SysKeycodes.h"

#include "IRenderer.h"

#include "EventHandler.h"
#include "EventToken.h"
#include "EventFwd.h"
#include "EventFuncFwd.h"

#include "MouseDownEvent.h"
#include "MouseUpEvent.h"
#include "MouseMotionEVent.h"

#include "TextureFactory.h"
#include "ITexture.h"

#include "ZoneBase.h"
#include "Sprite.h"
#include "EntityBase.h"
#include "EditorCameraController.h"

#include "ImGuiInputHandler.h"
#include "ImGuiInterfaceDrawer.h"
#include "ImGuiRenderer.h"

namespace
{
    class SpriteDrawable : public mono::EntityBase
    {
    public:

        SpriteDrawable(const char* file)
        : m_sprite(file)
        {
            mPosition = math::Vector2f(0, 0);
            mScale = math::Vector2f(50, 50);
            m_sprite.SetAnimation(0);
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
}

using namespace editor;

EditorZone::EditorZone(const math::Vector2f& window_size, mono::EventHandler& event_handler)
    : m_windowSize(window_size),
      m_eventHandler(event_handler),
      m_inputHandler(event_handler),
      m_polygonTool(this),
      m_rotateTool(this),
      m_activeTool(&m_polygonTool)
{
    using namespace std::placeholders;

    const event::MouseDownEventFunc& mouse_down = std::bind(&EditorZone::OnMouseDown, this, _1);
    m_mouseDownToken = m_eventHandler.AddListener(mouse_down);

    const event::MouseUpEventFunc& mouse_up = std::bind(&EditorZone::OnMouseUp, this, _1);
    m_mouseUpToken = m_eventHandler.AddListener(mouse_up);

    const event::MouseMotionEventFunc& mouse_move = std::bind(&EditorZone::OnMouseMove, this, _1);
    m_mouseMoveToken = m_eventHandler.AddListener(mouse_move);

    m_context.showContextMenu = false;
    m_context.contextMenuCallback = std::bind(&EditorZone::OnContextMenu, this, _1);
    m_context.contextMenuItems = { "Polygon", "Decoration" };

    m_context.selectedPolygonIndex = -1;
    m_context.polygonSelected = std::bind(&EditorZone::OnSelectedPolygon, this, _1);
    m_context.polygonDeleted = std::bind(&EditorZone::OnDeletePolygon, this, _1);

    m_context.editorMenuCallback = std::bind(&EditorZone::EditorMenuCallback, this, _1);
    m_context.toolsMenuCallback = std::bind(&EditorZone::ToolsMenuCallback, this, _1);

    m_context.notifications = { { 1, "Hello" }, { 1, "Yeah" }, { 1, "Apa" } };

    m_interfaceDrawer = std::make_shared<editor::ImGuiInterfaceDrawer>(m_context);
}

EditorZone::~EditorZone()
{
    m_eventHandler.RemoveListener(m_mouseDownToken);
    m_eventHandler.RemoveListener(m_mouseUpToken);
    m_eventHandler.RemoveListener(m_mouseMoveToken);
}

void EditorZone::OnLoad(mono::ICameraPtr camera)
{
    m_camera = camera;
    m_cameraController = std::make_shared<editor::CameraController>(m_camera, m_windowSize, m_eventHandler);

    mono::ITexturePtr texture = mono::CreateTexture("cacodemon.png");

    std::unordered_map<unsigned int, mono::ITexturePtr> textures;
    textures.insert(std::make_pair(texture->Id(), texture));

    AddUpdatable(m_interfaceDrawer);
    AddDrawable(std::make_shared<editor::ImGuiRenderer>(m_windowSize, textures), 1);
    AddEntity(std::make_shared<SpriteDrawable>("shuttle.sprite"), 0);
}

void EditorZone::OnUnload()
{ }

void EditorZone::AddPolygon(const std::shared_ptr<editor::PolygonEntity>& polygon)
{
    AddEntity(polygon, 1);
    m_polygons.push_back(polygon);
    m_context.polygonItems.push_back("Polygon: " + std::to_string(m_polygons.size()));
}

bool EditorZone::OnMouseDown(const event::MouseDownEvent& event)
{
    if(event.key == MouseButton::LEFT)
    {
        m_activeTool->HandleMouseDown(math::Vector2f(event.worldX, event.worldY));
    }

    return m_activeTool->IsActive();
}

bool EditorZone::OnMouseUp(const event::MouseUpEvent& event)
{
    if(event.key == MouseButton::LEFT)
    {
        m_activeTool->HandleMouseUp(math::Vector2f(event.worldX, event.worldY));
    }
    else if(event.key == MouseButton::RIGHT)
    {
        m_context.showContextMenu = true;
    }

    return m_activeTool->IsActive();
}

bool EditorZone::OnMouseMove(const event::MouseMotionEvent& event)
{
    m_activeTool->HandleMousePosition(math::Vector2f(event.worldX, event.worldY));
    return m_activeTool->IsActive();
}

void EditorZone::OnContextMenu(int index)
{
    if(index == 0)
        m_activeTool->IsActive() ? m_activeTool->End() : m_activeTool->Start();
}

void EditorZone::OnSelectedPolygon(int index)
{
    m_context.selectedPolygonIndex = index;

    for(int polygon_index = 0; polygon_index < m_polygons.size(); ++polygon_index)
        m_polygons[polygon_index]->m_selected = (polygon_index == index);
}

void EditorZone::OnDeletePolygon(int index)
{
    const auto remove_polygon_func = [this, index] {
        std::shared_ptr<editor::PolygonEntity> polygon = m_polygons[index];

        m_polygons.erase(m_polygons.begin() + index);
        this->RemoveEntity(polygon);

        m_context.polygonItems.erase(m_context.polygonItems.begin() + index);
    };

    SchedulePreFrameTask(remove_polygon_func);
}

void EditorZone::EditorMenuCallback(int index)
{
    std::printf("Editor %u\n", index);
}

void EditorZone::ToolsMenuCallback(int index)
{
    if(index == 0)
        m_activeTool = &m_polygonTool;
    else if(index == 1)
        m_activeTool = &m_rotateTool;
}

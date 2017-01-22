//
//  Editor.cpp
//  MonoiOS
//
//  Created by Niklas Damberg on 02/07/16.
//
//

#include "Editor.h"

#include "IRenderer.h"
#include "ICamera.h"
#include "IWindow.h"

#include "Texture/TextureFactory.h"
#include "Texture/ITexture.h"

#include "ZoneBase.h"
#include "Sprite/ISprite.h"
#include "Sprite/SpriteFactory.h"
#include "EntityBase.h"
#include "Color.h"
#include "Math/Matrix.h"
#include "Math/MathFunctions.h"

#include "UserInputController.h"

#include "ImGuiInterfaceDrawer.h"
#include "ImGuiImpl/ImGuiRenderer.h"

#include "Events/EventFuncFwd.h"
#include "Events/SurfaceChangedEvent.h"
#include "EventHandler/EventHandler.h"

#include "EditorConfig.h"
#include "WorldSerializer.h"

#include "Visualizers/GridVisualizer.h"
#include "Visualizers/GrabberVisualizer.h"

namespace
{
    class SpriteDrawable : public mono::EntityBase
    {
    public:
        SpriteDrawable(const char* file)
        {
            m_sprite = mono::CreateSprite(file);
            SetScale(math::Vector(1, 1));
        }
        virtual void Draw(mono::IRenderer& renderer) const
        {
            renderer.DrawSprite(*m_sprite);
        }
        virtual void Update(unsigned int delta)
        {
            m_sprite->doUpdate(delta);
        }
        mono::ISpritePtr m_sprite;
    };

    const char* avalible_textures[] = {
        "textures/placeholder.png",
        "textures/stone_tileable.png",
        "textures/gray_stone.png",
        "textures/dark_stone.png",
        "textures/brown_stone.png",
        "textures/lava1.png",
        "textures/lava2.png"
        };

    constexpr int n_textures = 7;

    int FindTextureIndex(const char* texture)
    {
        for(int index = 0; index < n_textures; ++index)
        {
            const bool found = std::strstr(texture, avalible_textures[index]) != nullptr;
            if(found)
                return index;
        }

        return -1;
    }

    void SetupIcons(editor::UIContext& context, std::unordered_map<unsigned int, mono::ITexturePtr>& textures)
    {
        mono::ITexturePtr texture = mono::CreateTexture("textures/placeholder.png");
        textures.insert(std::make_pair(texture->Id(), texture));

        context.tools_texture_id = texture->Id();
        context.default_icon = math::Quad(0.0f, 0.0f, 1.0f, 1.0f);
    }
}

using namespace editor;

Editor::Editor(const mono::IWindowPtr& window, mono::EventHandler& event_handler, const char* file_name)
    : m_window(window),
      m_eventHandler(event_handler),
      m_inputHandler(event_handler),
      m_fileName(file_name)
{
    using namespace std::placeholders;

    m_context.texture_items_count = n_textures;
    m_context.texture_items = avalible_textures;

    m_context.contextMenuCallback = std::bind(&Editor::OnContextMenu, this, _1);
    m_context.texture_repeate_callback = std::bind(&Editor::OnTextureRepeate, this, _1);
    m_context.texture_changed_callback = std::bind(&Editor::OnTextureChanged, this, _1);
    m_context.path_name_callback = std::bind(&Editor::OnPathName, this, _1);
    m_context.delete_callback = std::bind(&Editor::OnDeletePolygon, this);
    m_context.editorMenuCallback = std::bind(&Editor::EditorMenuCallback, this, _1);
    m_context.toolsMenuCallback = std::bind(&Editor::ToolsMenuCallback, this, _1);

    std::unordered_map<unsigned int, mono::ITexturePtr> textures;
    SetupIcons(m_context, textures);

    m_guiRenderer = std::make_shared<ImGuiRenderer>("editor_imgui.ini", m_window->Size(), textures);

    const event::SurfaceChangedEventFunc surface_func = std::bind(&Editor::OnSurfaceChanged, this, _1);
    m_surfaceChangedToken = m_eventHandler.AddListener(surface_func);

    const auto& polygons = LoadPolygons(m_fileName);
    for(auto& polygon : polygons)
        AddPolygon(polygon);

    m_paths = LoadPaths("world.paths");
    for(auto& path : m_paths)
        AddEntity(path, 1);
}

Editor::~Editor()
{
    m_eventHandler.RemoveListener(m_surfaceChangedToken);

    SavePolygons(m_fileName, m_polygons);
    SavePaths("world.paths", m_paths);

    editor::Config config;
    config.cameraPosition = m_camera->GetPosition();
    config.cameraViewport = m_camera->GetViewport();

    editor::SaveConfig("editor_config.json", config);
}

void Editor::OnLoad(mono::ICameraPtr camera)
{
    m_camera = camera;
    
    editor::Config config;
    const bool config_loaded = editor::LoadConfig("editor_config.json", config);
    if(config_loaded)
    {
        camera->SetPosition(config.cameraPosition);
        camera->SetViewport(config.cameraViewport);
    }

    m_userInputController = std::make_shared<editor::UserInputController>(camera, m_window, this, &m_context, m_eventHandler);

    AddUpdatable(std::make_shared<editor::ImGuiInterfaceDrawer>(m_context));
    AddDrawable(m_guiRenderer, 2);
    AddDrawable(std::make_shared<GridVisualizer>(camera), 0);
    AddDrawable(std::make_shared<GrabberVisualizer>(m_grabbers), 2);
    AddEntity(std::make_shared<SpriteDrawable>("sprites/shuttle.sprite"), 0);
}

void Editor::OnUnload()
{ }

bool Editor::OnSurfaceChanged(const event::SurfaceChangedEvent& event)
{
    if(event.width > 0 && event.height > 0)
        m_guiRenderer->SetWindowSize(math::Vector(event.width, event.height));

    return false;
}

void Editor::UpdateUI()
{
    m_context.has_polygon_selection = (m_selected_polygon != nullptr);
    m_context.has_path_selection    = (m_selected_path != nullptr);

    if(m_selected_polygon)
    {
        const math::Vector& position = m_selected_polygon->Position();
        m_context.position_x = position.x;
        m_context.position_y = position.y;
        m_context.rotation = m_selected_polygon->Rotation();
        m_context.texture_repeate = m_selected_polygon->GetTextureRepate();
        m_context.texture_index = FindTextureIndex(m_selected_polygon->GetTexture());
    }
    else if(m_selected_path)
    {
        const math::Vector& position = m_selected_path->Position();
        m_context.position_x = position.x;
        m_context.position_y = position.y;
        m_context.rotation = 0.0f;
        m_context.path_name = m_selected_path->m_name.c_str();
    }
}

void Editor::AddPolygon(const std::shared_ptr<editor::PolygonEntity>& polygon)
{
    AddEntity(polygon, 1);
    m_polygons.push_back(polygon);
}

void Editor::AddPath(const std::shared_ptr<editor::PathEntity>& path)
{
    AddEntity(path, 1);
    m_paths.push_back(path);
}

void Editor::SelectEntity(const mono::IEntityPtr& entity)
{
    m_selected_polygon = nullptr;
    m_selected_path = nullptr;

    for(auto& polygon : m_polygons)
    {
        const bool selected = (polygon == entity);
        polygon->SetSelected(selected);
        if(selected)
            m_selected_polygon = polygon;
    }

    for(auto& path : m_paths)
    {
        const bool selected = (path == entity);
        path->SetSelected(selected);
        if(selected)
            m_selected_path = path;
    }

    UpdateGrabbers();
    UpdateUI();
}

void Editor::SelectGrabber(const math::Vector& position)
{
    for(auto& grabber : m_grabbers)
        grabber.hoover = false;

    editor::Grabber* grabber = FindGrabber(position);
    if(grabber)
        grabber->hoover = true;
}

editor::Grabber* Editor::FindGrabber(const math::Vector& position)
{
    const float threshold = m_camera->GetViewport().mB.x / m_window->Size().x * 5.0f;

    for(auto& grabber : m_grabbers)
    {
        const float distance = math::Length(grabber.position - position);
        if(distance <= threshold)
            return &grabber;
    }

    return nullptr;
}

void Editor::UpdateGrabbers()
{
    using namespace std::placeholders;

    m_grabbers.clear();

    if(m_selected_polygon)
    {
        const math::Matrix& transform = m_selected_polygon->Transformation();

        const auto& vertices = m_selected_polygon->GetVertices();
        for(size_t index = 0; index < vertices.size(); ++index)
        {
            Grabber grab;
            grab.position = math::Transform(transform, vertices[index]);
            grab.callback = std::bind(&PolygonEntity::SetVertex, m_selected_polygon, _1, index);
            m_grabbers.push_back(grab);
        }
    }
    else if(m_selected_path)
    {
        const math::Matrix& transform = m_selected_path->Transformation();

        const auto& vertices = m_selected_path->m_points;
        for(size_t index = 0; index < vertices.size(); ++index)
        {
            Grabber grab;
            grab.position = math::Transform(transform, vertices[index]);
            grab.callback = std::bind(&PathEntity::SetVertex, m_selected_path, _1, index);
            m_grabbers.push_back(grab);
        }
    }
}

void Editor::OnContextMenu(int index)
{
    m_userInputController->HandleContextMenu(index);
}

void Editor::OnTextureRepeate(float repeate)
{
    m_selected_polygon->SetTextureRepeate(repeate);
}

void Editor::OnTextureChanged(int texture_index)
{
    m_selected_polygon->SetTexture(avalible_textures[texture_index]);
}

void Editor::OnPathName(const char* new_name)
{
    m_selected_path->m_name = new_name;
}

void Editor::OnDeletePolygon()
{
    m_context.has_polygon_selection = false;
    m_context.has_path_selection = false;
    m_grabbers.clear();

    if(m_selected_polygon)
    {
        const auto remove_polygon_func = [this] {
            auto it = std::find(m_polygons.begin(), m_polygons.end(), m_selected_polygon);
            m_polygons.erase(it);
            RemoveEntity(m_selected_polygon);
            m_selected_polygon = nullptr;
        };

        SchedulePreFrameTask(remove_polygon_func);
    }
    else if(m_selected_path)
    {
        const auto remove_path_func = [this] {
            auto it = std::find(m_paths.begin(), m_paths.end(), m_selected_path);
            m_paths.erase(it);
            RemoveEntity(m_selected_path);
            m_selected_path = nullptr;
        };

        SchedulePreFrameTask(remove_path_func);
    }
}

void Editor::EditorMenuCallback(EditorMenuOptions option)
{
    if(option == EditorMenuOptions::SAVE)
    {
        SavePolygons(m_fileName, m_polygons);
        SavePaths("world.paths", m_paths);
    }
}

void Editor::ToolsMenuCallback(ToolsMenuOptions option)
{
    m_userInputController->SelectTool(option);
}

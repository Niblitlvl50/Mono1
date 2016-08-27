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

#include "TextureFactory.h"
#include "ITexture.h"

#include "ZoneBase.h"
#include "ISprite.h"
#include "SpriteFactory.h"
#include "EntityBase.h"
#include "Color.h"
#include "Matrix.h"

#include "UserInputController.h"

#include "ImGuiInputHandler.h"
#include "ImGuiInterfaceDrawer.h"
#include "ImGuiRenderer.h"

#include "EventFuncFwd.h"
#include "EventHandler.h"
#include "SurfaceChangedEvent.h"

#include "WorldFile.h"
#include "EditorConfig.h"

#include "GridVisualizer.h"

namespace
{
    class SpriteDrawable : public mono::EntityBase
    {
    public:

        SpriteDrawable(const char* file)
        {
            m_sprite = mono::CreateSprite(file);
            SetScale(math::Vector2f(50, 50));
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

    std::vector<std::shared_ptr<editor::PolygonEntity>> LoadPolygons(const char* file_name)
    {
        std::vector<std::shared_ptr<editor::PolygonEntity>> polygon_data;

        if(!file_name)
            return polygon_data;

        File::FilePtr file;

        try
        {
            file = File::OpenBinaryFile(file_name);
        }
        catch(...)
        {
            return polygon_data;
        }

        world::LevelFileHeader level_data;
        world::ReadWorld(file, level_data);

        polygon_data.reserve(level_data.polygons.size());

        for(const world::PolygonData& polygon : level_data.polygons)
        {
            std::shared_ptr<editor::PolygonEntity> polygon_entity = std::make_shared<editor::PolygonEntity>();
            polygon_entity->SetPosition(polygon.position);
            polygon_entity->SetBasePoint(polygon.local_offset);
            polygon_entity->SetRotation(polygon.rotation);

            math::Matrix invert_transform = polygon_entity->Transformation();
            math::Inverse(invert_transform);

            for(const math::Vector2f& vertex : polygon.vertices)
                polygon_entity->AddVertex(math::Transform(invert_transform, vertex));

            polygon_data.push_back(polygon_entity);
        }

        return polygon_data;
    }

    void SavePolygons(const char* file_name, const std::vector<std::shared_ptr<editor::PolygonEntity>>& polygons)
    {
        if(!file_name)
            return;

        world::LevelFileHeader world_data;
        world_data.version = 1;
        world_data.polygons.resize(polygons.size());

        for(int index = 0; index < polygons.size(); ++index)
        {
            const auto& polygon_entity = polygons[index];
            world::PolygonData& polygon_data = world_data.polygons[index];

            polygon_data.position = polygon_entity->Position();
            polygon_data.local_offset = polygon_entity->BasePoint();
            polygon_data.rotation = polygon_entity->Rotation();

            const math::Matrix& transform = polygon_entity->Transformation();

            for(const math::Vector2f& vertex : polygon_entity->GetVertices())
                polygon_data.vertices.push_back(math::Transform(transform, vertex));
        }

        File::FilePtr file = File::CreateBinaryFile(file_name);
        world::WriteWorld(file, world_data);
    }
}

using namespace editor;

EditorZone::EditorZone(const math::Vector2f& window_size, mono::EventHandler& event_handler, const char* file_name)
    : m_windowSize(window_size),
      m_eventHandler(event_handler),
      m_inputHandler(event_handler),
      m_fileName(file_name)
{
    using namespace std::placeholders;

    m_context.showContextMenu = false;
    m_context.contextMenuCallback = std::bind(&EditorZone::OnContextMenu, this, _1);
    m_context.contextMenuItems = { "Polygon", "Decoration" };

    m_context.selectedPolygonIndex = -1;
    m_context.polygonSelected = std::bind(&EditorZone::OnSelectedPolygon, this, _1);
    m_context.polygonDeleted = std::bind(&EditorZone::OnDeletePolygon, this, _1);

    m_context.editorMenuCallback = std::bind(&EditorZone::EditorMenuCallback, this, _1);
    m_context.toolsMenuCallback = std::bind(&EditorZone::ToolsMenuCallback, this, _1);

    m_interfaceDrawer = std::make_shared<editor::ImGuiInterfaceDrawer>(m_context);

    mono::ITexturePtr texture = mono::CreateTexture("placeholder.png");
    std::unordered_map<unsigned int, mono::ITexturePtr> textures;
    textures.insert(std::make_pair(texture->Id(), texture));

    m_guiRenderer = std::make_shared<editor::ImGuiRenderer>(m_windowSize, textures);

    const event::SurfaceChangedEventFunc surface_func = std::bind(&EditorZone::OnSurfaceChanged, this, _1);
    m_surfaceChangedToken = m_eventHandler.AddListener(surface_func);

    const auto& polygons = LoadPolygons(m_fileName);
    for(auto& polygon : polygons)
        AddPolygon(polygon);
}

EditorZone::~EditorZone()
{
    m_eventHandler.RemoveListener(m_surfaceChangedToken);

    SavePolygons(m_fileName, m_polygons);

    editor::Config config;
    config.cameraPosition = m_camera->GetPosition();
    config.cameraViewport = m_camera->GetViewport();

    editor::SaveConfig("editor.config", config);
}

void EditorZone::OnLoad(mono::ICameraPtr camera)
{
    m_camera = camera;
    
    editor::Config config;
    const bool config_loaded = editor::LoadConfig("editor.config", config);
    if(config_loaded)
    {
        camera->SetPosition(config.cameraPosition);
        camera->SetViewport(config.cameraViewport);
    }

    m_userInputController = std::make_shared<editor::UserInputController>(camera, this, &m_context, m_windowSize, m_eventHandler);

    AddUpdatable(m_interfaceDrawer);

    AddDrawable(m_guiRenderer, 2);
    AddDrawable(std::make_shared<GridVisualizer>(camera), 0);

    AddEntity(std::make_shared<SpriteDrawable>("shuttle.sprite"), 0);
}

void EditorZone::OnUnload()
{ }

bool EditorZone::OnSurfaceChanged(const event::SurfaceChangedEvent& event)
{
    if(event.width > 0 && event.height > 0)
        m_guiRenderer->SetWindowSize(math::Vector2f(event.width, event.height));

    return false;
}

void EditorZone::AddPolygon(const std::shared_ptr<editor::PolygonEntity>& polygon)
{
    AddEntity(polygon, 1);
    m_polygons.push_back(polygon);
    m_context.polygonItems.push_back("Polygon: " + std::to_string(m_polygons.size()));
}

void EditorZone::OnContextMenu(int index)
{
    m_userInputController->HandleContextMenu(index);
}

void EditorZone::OnSelectedPolygon(int index)
{
    m_context.selectedPolygonIndex = index;

    for(int polygon_index = 0; polygon_index < m_polygons.size(); ++polygon_index)
        m_polygons[polygon_index]->SetSelected(polygon_index == index);
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

void EditorZone::EditorMenuCallback(EditorMenuOptions option)
{
    if(option == EditorMenuOptions::SAVE)
        SavePolygons(m_fileName, m_polygons);
}

void EditorZone::ToolsMenuCallback(ToolsMenuOptions option)
{
    m_userInputController->SelectTool(option);
}

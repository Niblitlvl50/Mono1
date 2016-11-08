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

#include "WorldFile.h"
#include "EditorConfig.h"

#include "Visualizers/GridVisualizer.h"

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
            polygon_entity->SetTextureRepeate(polygon.texture_repeate);
            polygon_entity->SetTexture(polygon.texture);

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

        for(size_t index = 0; index < polygons.size(); ++index)
        {
            const auto& polygon_entity = polygons[index];
            world::PolygonData& polygon_data = world_data.polygons[index];

            polygon_data.position = polygon_entity->Position();
            polygon_data.local_offset = polygon_entity->BasePoint();
            polygon_data.rotation = polygon_entity->Rotation();
            polygon_data.texture_repeate = polygon_entity->GetTextureRepate();

            const char* texture = polygon_entity->GetTexture();
            const std::size_t string_length = std::strlen(texture);

            std::memcpy(polygon_data.texture, texture, string_length);

            const math::Matrix& transform = polygon_entity->Transformation();

            for(const math::Vector2f& vertex : polygon_entity->GetVertices())
                polygon_data.vertices.push_back(math::Transform(transform, vertex));
        }

        File::FilePtr file = File::CreateBinaryFile(file_name);
        world::WriteWorld(file, world_data);
    }

    const char* avalible_textures[] = {
        "textures/stone_tileable.png",
        "textures/gray_stone.png",
        "textures/dark_stone.png",
        "textures/brown_stone.png",
        "textures/lava1.png",
        "textures/lava2.png"
        };

    constexpr int n_textures = 6;

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

    m_context.showContextMenu = false;
    m_context.texture_items_count = n_textures;
    m_context.texture_items = avalible_textures;
    m_context.has_selection = false;

    m_context.contextMenuCallback = std::bind(&Editor::OnContextMenu, this, _1);
    m_context.texture_repeate_callback = std::bind(&Editor::OnTextureRepeate, this, _1);
    m_context.texture_changed_callback = std::bind(&Editor::OnTextureChanged, this, _1);
    m_context.delete_callback = std::bind(&Editor::OnDeletePolygon, this);
    m_context.editorMenuCallback = std::bind(&Editor::EditorMenuCallback, this, _1);
    m_context.toolsMenuCallback = std::bind(&Editor::ToolsMenuCallback, this, _1);

    std::unordered_map<unsigned int, mono::ITexturePtr> textures;
    SetupIcons(m_context, textures);

    m_interfaceDrawer = std::make_shared<editor::ImGuiInterfaceDrawer>(m_context);
    m_guiRenderer = std::make_shared<ImGuiRenderer>("editor_imgui.ini", m_window->Size(), textures);

    const event::SurfaceChangedEventFunc surface_func = std::bind(&Editor::OnSurfaceChanged, this, _1);
    m_surfaceChangedToken = m_eventHandler.AddListener(surface_func);

    const auto& polygons = LoadPolygons(m_fileName);
    for(auto& polygon : polygons)
        AddPolygon(polygon);
}

Editor::~Editor()
{
    m_eventHandler.RemoveListener(m_surfaceChangedToken);

    SavePolygons(m_fileName, m_polygons);

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

    AddUpdatable(m_interfaceDrawer);
    AddDrawable(m_guiRenderer, 2);
    AddDrawable(std::make_shared<GridVisualizer>(camera), 0);
    AddEntity(std::make_shared<SpriteDrawable>("sprites/shuttle.sprite"), 0);
}

void Editor::OnUnload()
{ }

bool Editor::OnSurfaceChanged(const event::SurfaceChangedEvent& event)
{
    if(event.width > 0 && event.height > 0)
        m_guiRenderer->SetWindowSize(math::Vector2f(event.width, event.height));

    return false;
}

void Editor::AddPolygon(const std::shared_ptr<editor::PolygonEntity>& polygon)
{
    AddEntity(polygon, 1);
    m_polygons.push_back(polygon);
}

void Editor::AddPath(const std::vector<math::Vector2f>& points)
{

}

mono::IEntityPtr Editor::FindEntityFromPosition(const math::Vector2f& position)
{
    for(auto& polygon : m_polygons)
    {
        const math::Quad& bb = polygon->BoundingBox();
        const bool found_polygon = math::PointInsideQuad(position, bb);
        if(found_polygon)
            return polygon;
    }

    return nullptr;
}

void Editor::SelectEntity(const mono::IEntityPtr& entity)
{
    m_selected_polygon = nullptr;

    for(auto& polygon : m_polygons)
    {
        const bool selected = (polygon == entity);
        polygon->SetSelected(selected);
        if(selected)
            m_selected_polygon = polygon;
    }

    m_context.has_selection = (m_selected_polygon != nullptr);
    if(m_selected_polygon)
    {
        const math::Vector2f& position = m_selected_polygon->Position();
        m_context.polygon_x = position.x;
        m_context.polygon_y = position.y;
        m_context.polygon_rotation = m_selected_polygon->Rotation();
        m_context.texture_repeate = m_selected_polygon->GetTextureRepate();

        const char* texture = m_selected_polygon->GetTexture();

        for(int index = 0; index < n_textures; ++index)
        {
            const bool found = std::strstr(texture, avalible_textures[index]) != nullptr;
            if(found)
            {
                m_context.texture_index = index;
                break;
            }
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

void Editor::OnDeletePolygon()
{
    m_context.has_selection = false;

    const auto remove_polygon_func = [this] {
        auto it = std::find(m_polygons.begin(), m_polygons.end(), m_selected_polygon);
        m_polygons.erase(it);
        this->RemoveEntity(m_selected_polygon);
        m_selected_polygon = nullptr;
    };

    SchedulePreFrameTask(remove_polygon_func);
}

void Editor::EditorMenuCallback(EditorMenuOptions option)
{
    if(option == EditorMenuOptions::SAVE)
        SavePolygons(m_fileName, m_polygons);
}

void Editor::ToolsMenuCallback(ToolsMenuOptions option)
{
    m_userInputController->SelectTool(option);
}

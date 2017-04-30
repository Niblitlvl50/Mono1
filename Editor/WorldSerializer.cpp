
#include "WorldSerializer.h"
#include "Polygon.h"
#include "Path.h"
#include "SpriteEntity.h"
#include "Prefab.h"
#include "Math/Matrix.h"
#include "Math/Serialize.h"
#include "System/File.h"
#include "WorldFile.h"
#include "EntityRepository.h"

#include "Paths/IPath.h"
#include "Paths/PathFactory.h"

#include "nlohmann_json/json.hpp"

std::vector<std::shared_ptr<editor::PolygonEntity>> editor::LoadPolygons(const char* file_name)
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
        polygon_entity->SetTexture(polygon.texture);

        math::Matrix invert_transform = polygon_entity->Transformation();
        math::Inverse(invert_transform);

        for(const math::Vector& vertex : polygon.vertices)
            polygon_entity->AddVertex(math::Transform(invert_transform, vertex));

        if(!polygon.vertices.empty())
            polygon_data.push_back(polygon_entity);
    }

    return polygon_data;
}

void editor::SavePolygons(const char* file_name, const std::vector<std::shared_ptr<editor::PolygonEntity>>& polygons)
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

        const char* texture = polygon_entity->GetTexture();
        const std::size_t string_length = std::strlen(texture);

        std::memcpy(polygon_data.texture, texture, string_length);

        const math::Matrix& transform = polygon_entity->Transformation();

        for(const math::Vector& vertex : polygon_entity->GetVertices())
            polygon_data.vertices.push_back(math::Transform(transform, vertex));
    }

    File::FilePtr file = File::CreateBinaryFile(file_name);
    world::WriteWorld(file, world_data);
}

std::vector<std::shared_ptr<editor::PathEntity>> editor::LoadPaths(const char* file_name)
{
    std::vector<std::shared_ptr<editor::PathEntity>> paths;

    File::FilePtr file = File::OpenAsciiFile(file_name);
    if(!file)
        return paths;

    std::vector<byte> file_data;
    File::FileRead(file, file_data);

    const nlohmann::json& json = nlohmann::json::parse(file_data);
    const std::vector<std::string>& path_names = json["path_files"];

    paths.reserve(path_names.size());

    const auto get_name = [](const std::string& file_name) {
        const size_t slash_pos = file_name.find_last_of('/');
        const size_t dot_pos = file_name.find_last_of('.');
        return file_name.substr(slash_pos +1, dot_pos - slash_pos -1);
    };

    for(const std::string& file : path_names)
    {
        std::shared_ptr<mono::IPath> path = mono::CreatePath(file.c_str());
        auto path_entity = std::make_shared<editor::PathEntity>(get_name(file), path->GetPathPoints());
        path_entity->SetPosition(path->GetGlobalPosition());
        paths.push_back(path_entity);
    }

    return paths;
}

void editor::SavePaths(const char* file_name, const std::vector<std::shared_ptr<editor::PathEntity>>& paths)
{
    std::vector<std::string> path_names;
    path_names.reserve(paths.size());

    for(auto& path : paths)
    {
        const std::string& filename = "paths/" + path->m_name + ".path";
        mono::SavePath(filename.c_str(), path->Position(), path->m_points);
        path_names.push_back(filename);
    }

    nlohmann::json json;
    json["path_files"] = path_names;

    const std::string& serialized_json = json.dump(4);

    File::FilePtr file = File::CreateAsciiFile(file_name);
    std::fwrite(serialized_json.data(), serialized_json.length(), sizeof(char), file.get());
}

void editor::SaveObjects(const char* file_name, const std::vector<std::shared_ptr<editor::SpriteEntity>>& objects)
{
    nlohmann::json json_object_collection;

    for(auto& object : objects)
    {
        nlohmann::json json_object;
        json_object["name"] = object->Name();
        json_object["position"] = object->Position();
        //json_object["scale"] = object->Scale();
        json_object["rotation"] = object->Rotation();

        json_object_collection.push_back(json_object);
    }

    nlohmann::json json;
    json["objects"] = json_object_collection;

    const std::string& serialized_json = json.dump(4);

    File::FilePtr file = File::CreateAsciiFile(file_name);
    std::fwrite(serialized_json.data(), serialized_json.length(), sizeof(char), file.get());
}

std::vector<std::shared_ptr<editor::SpriteEntity>> editor::LoadObjects(const char* file_name, const editor::EntityRepository& entity_repo)
{
    std::vector<std::shared_ptr<editor::SpriteEntity>> objects;

    File::FilePtr file = File::OpenAsciiFile(file_name);
    if(!file)
        return objects;

    std::vector<byte> file_data;
    File::FileRead(file, file_data);

    const nlohmann::json& json = nlohmann::json::parse(file_data);
    const nlohmann::json& json_objects = json["objects"];

    for(const auto& json_object : json_objects)
    {
        const std::string& name = json_object["name"];
        const math::Vector& position = json_object["position"];
        const float rotation = json_object["rotation"];

        const editor::EntityDefinition& def = entity_repo.GetDefinitionFromName(name);

        auto sprite_object = std::make_shared<editor::SpriteEntity>(name.c_str(), def.sprite_file.c_str());
        sprite_object->SetPosition(position);
        sprite_object->SetScale(def.scale);
        sprite_object->SetRotation(rotation);

        objects.push_back(sprite_object);
    }

    return objects;
}

void editor::SavePrefabs(const char* file_name, const std::vector<std::shared_ptr<editor::Prefab>>& prefabs)
{

}

std::vector<std::shared_ptr<editor::Prefab>> editor::LoadPrefabs(const char* file_name, const editor::EntityRepository& entity_repo)
{
    std::vector<std::shared_ptr<editor::Prefab>> prefabs;

    File::FilePtr file = File::OpenAsciiFile(file_name);
    if(!file)
        return prefabs;

    std::vector<byte> file_data;
    File::FileRead(file, file_data);

    const nlohmann::json& json = nlohmann::json::parse(file_data);
    const nlohmann::json& json_prefabs = json["prefabs"];

    for(const auto& json_prefab : json_prefabs)
    {
        const std::string& name = json_prefab["name"];
        const math::Vector& position = json_prefab["position"];
        const float rotation = json_prefab["rotation"];

        const PrefabDefinition& def = entity_repo.GetPrefabFromName(name);
        
        auto prefab_object = std::make_shared<editor::Prefab>(name.c_str(), def.sprite_file.c_str(), def.snap_points);
        prefab_object->SetPosition(position);
        prefab_object->SetScale(def.scale);
        prefab_object->SetRotation(rotation);

        prefabs.push_back(prefab_object);
    }

    return prefabs;
}


#include "EntityRepository.h"
#include "System/SysFile.h"
#include "Math/Serialize.h"
#include "nlohmann_json/json.hpp"

using namespace editor;

namespace
{
    bool LoadEntities(std::vector<EntityDefinition>& collection)
    {
        File::FilePtr file = File::OpenAsciiFile("editor_entities.json");
        if(!file)
            return false;

        std::vector<byte> file_data;
        File::FileRead(file, file_data);
        const nlohmann::json& json = nlohmann::json::parse(file_data);

        const nlohmann::json& entities = json["entities"];
        collection.reserve(entities.size());

        for(auto& object : entities)
        {
            EntityDefinition definition;
            definition.name = object["name"];
            definition.sprite_file = object["sprite"];
            definition.scale = object["scale"];
            collection.push_back(definition);
        }

        return true;
    }

    bool LoadPrefabs(std::vector<PrefabDefinition>& collection)
    {
        File::FilePtr file = File::OpenAsciiFile("editor_prefabs.json");
        if(!file)
            return false;

        std::vector<byte> file_data;
        File::FileRead(file, file_data);
        const nlohmann::json& json = nlohmann::json::parse(file_data);

        const nlohmann::json& prefabs = json["prefabs"];
        collection.reserve(prefabs.size());

        for(const auto& prefab : prefabs)
        {
            PrefabDefinition definition;
            definition.name = prefab["name"];
            definition.sprite_file = prefab["sprite"];
            definition.scale = prefab["scale"];

            const nlohmann::json& collision_shape = prefab["collision_shape"];
            definition.collision_shape.reserve(collision_shape.size());

            for(const auto& point : collision_shape)
                definition.collision_shape.push_back(point);

            const nlohmann::json& snap_points = prefab["snap_points"];
            definition.snap_points.reserve(snap_points.size());

            for(const auto& snap_point : snap_points)
            {
                SnapPoint sp;
                sp.normal = snap_point["normal"];
                sp.position = snap_point["position"];
                definition.snap_points.push_back(sp);
            }

            collection.push_back(definition);
        }

        return true;
    }
}

bool EntityRepository::LoadDefinitions()
{
    return LoadEntities(m_entities) && LoadPrefabs(m_prefabs);
}

const EntityDefinition& EntityRepository::GetDefinitionFromName(const std::string& name) const
{
    const auto find_func = [&name](const EntityDefinition& definition) {
        return definition.name == name;
    };

    return *std::find_if(m_entities.begin(), m_entities.end(), find_func);
}

const PrefabDefinition& EntityRepository::GetPrefabFromName(const std::string& name) const
{
    const auto find_func = [&name](const PrefabDefinition& definition) {
        return definition.name == name;
    };

    return *std::find_if(m_prefabs.begin(), m_prefabs.end(), find_func);
}

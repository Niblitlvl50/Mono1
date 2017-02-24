
#include "EntityRepository.h"
#include "System/SysFile.h"
#include "Math/Serialize.h"
#include "nlohmann_json/json.hpp"

using namespace editor;

namespace
{
    constexpr const char* entity_file = "editor_entities.json";
}

EntityRepository::EntityRepository()
{
}

bool EntityRepository::LoadDefinitions()
{
    File::FilePtr file = File::OpenAsciiFile(entity_file);
    if(!file)
        return false;

    std::vector<byte> file_data;
    File::FileRead(file, file_data);
    const nlohmann::json& json = nlohmann::json::parse(file_data);

    const nlohmann::json& entities = json["entities"];
    m_entities.reserve(entities.size());

    for(auto& object : entities)
    {
        EntityDefinition definition;
        definition.name = object["name"];
        definition.sprite_file = object["sprite"];
        definition.scale = object["scale"];
        m_entities.push_back(definition);
    }

    return true;
}

const EntityDefinition& EntityRepository::GetDefinitionFromName(const std::string& name) const
{
    const auto find_func = [&name](const EntityDefinition& definition) {
        return definition.name == name;
    };

    return *std::find_if(m_entities.begin(), m_entities.end(), find_func);
}

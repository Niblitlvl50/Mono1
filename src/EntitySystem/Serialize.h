
#pragma once

#include "EntityTypes.h"
#include "EntitySystem/ObjectAttribute.h"
#include "EntitySystem/EntitySystem.h"
#include "Math/Serialize.h"
#include "Rendering/Serialize.h"

#include "nlohmann/json.hpp"

namespace mono
{
    inline void to_json(nlohmann::json& json, const mono::Event& event)
    {
        json["type"] = event.type;
        json["text"] = event.text;
    }

    inline void from_json(const nlohmann::json& json, mono::Event& event)
    {
        event.type = json["type"];
        event.text = json["text"];
    }
}

inline void to_json(nlohmann::json& json, const Attribute& attribute)
{
    json["id"] = attribute.id;
    json["name"] = mono::EntitySystem::AttributeNameLookup(attribute.id);
    json["variant_type"] = attribute.value.index();

    const auto visitor = [&json](const auto& value)
    {
        json["variant_value"] = value;
    };
    std::visit(visitor, attribute.value);
}

inline void from_json(const nlohmann::json& json, Attribute& attribute)
{
    attribute.id = json["id"].get<uint32_t>();

    const uint32_t value_index = json["variant_type"].get<int>();
    switch(value_index)
    {
    case VariantTypeIndex::BOOL:
        attribute.value = json["variant_value"].get<bool>();
        break;
    case VariantTypeIndex::INT:
        attribute.value = json["variant_value"].get<int>();
        break;
    case VariantTypeIndex::UINT:
        attribute.value = json["variant_value"].get<uint32_t>();
        break;
    case VariantTypeIndex::FLOAT:
        attribute.value = json["variant_value"].get<float>();
        break;
    case VariantTypeIndex::POINT:
        attribute.value = json["variant_value"].get<math::Vector>();
        break;
    case VariantTypeIndex::COLOR:
        attribute.value = json["variant_value"].get<mono::Color::RGBA>();
        break;
    case VariantTypeIndex::STRING:
        attribute.value = json["variant_value"].get<std::string>();
        break;
    case VariantTypeIndex::POLYGON:
        attribute.value = json["variant_value"].get<std::vector<math::Vector>>();
        break;
    case VariantTypeIndex::INTERVAL:
        attribute.value = json["variant_value"].get<math::Interval>();
        break;
    case VariantTypeIndex::VALUE_SPREAD:
        attribute.value = json["variant_value"].get<math::ValueSpread>();
        break;
    case VariantTypeIndex::GRADIENT_4:
        attribute.value = json["variant_value"].get<mono::Color::Gradient<4>>();
        break;
    case VariantTypeIndex::EVENT:
        attribute.value = json["variant_value"].get<mono::Event>();
        break;
    }
}

namespace mono
{
    inline void to_json(nlohmann::json& j, const mono::EntityData& entity_data)
    {
        //j = nlohmann::json{ {"red", color.red}, {"green", color.green}, {"blue", color.blue}, {"alpha", color.alpha} };
    }

    inline void from_json(const nlohmann::json& json, mono::EntityData& entity_data)
    {
        entity_data.entity_uuid = json.value("uuid_hash", 0);
        entity_data.entity_name = json.value("name", "Unnamed");
        entity_data.entity_properties = json.value("entity_properties", 0);

        for(const nlohmann::json& component : json["components"])
        {
            mono::ComponentData component_data;
            component_data.hash = component["hash"];
            component_data.name = component["name"];

            for(const nlohmann::json& property : component["properties"])
                component_data.properties.push_back(property);

            entity_data.entity_components.push_back(std::move(component_data));
        }
    }
}


#pragma once

#include "Rendering/Color.h"
#include "nlohmann/json.hpp"

namespace mono
{
    namespace Color
    {
        inline void to_json(nlohmann::json& j, const mono::Color::RGBA& color)
        {
            j = nlohmann::json{ {"red", color.red}, {"green", color.green}, {"blue", color.blue}, {"alpha", color.alpha} };
        }

        inline void from_json(const nlohmann::json& json, mono::Color::RGBA& color)
        {
            color.red   = json["red"].get<float>();
            color.green = json["green"].get<float>();
            color.blue  = json["blue"].get<float>();
            color.alpha = json["alpha"].get<float>();
        }
    }
}

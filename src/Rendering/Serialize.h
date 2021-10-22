
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

        inline void to_json(nlohmann::json& j, const mono::Color::Gradient<4>& gradient)
        {
            j = nlohmann::json{
                {"t", {gradient.t[0], gradient.t[1], gradient.t[2], gradient.t[3]} },
                {"gradient", {gradient.color[0], gradient.color[1], gradient.color[2], gradient.color[3]} }
            };
        }

        inline void from_json(const nlohmann::json& json, mono::Color::Gradient<4>& gradient)
        {
            const std::vector<float>& t = json["t"].get<std::vector<float>>();
            const std::vector<mono::Color::RGBA>& colors = json["gradient"].get<std::vector<mono::Color::RGBA>>();

            for(size_t index = 0; index < 4; ++index)
            {
                gradient.t[index] = t[index];
                gradient.color[index] = colors[index];
            }
        }
    }
}

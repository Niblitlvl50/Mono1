
#pragma once

#include "Math/Interval.h"
#include "Math/Quad.h"
#include "Math/Vector.h"
#include "nlohmann/json.hpp"

namespace math
{
    // Vector
    inline void to_json(nlohmann::json& j, const math::Vector& vector)
    {
        j = nlohmann::json{ {"x", vector.x}, {"y", vector.y} };
    }

    inline void from_json(const nlohmann::json& json, math::Vector& vector)
    {
        vector.x = json["x"].get<float>();
        vector.y = json["y"].get<float>();
    }

    // Quad
    inline void to_json(nlohmann::json& j, const math::Quad& quad)
    {
        j = nlohmann::json{
            {"x1", quad.mA.x}, {"y1", quad.mA.y},
            {"x2", quad.mB.x}, {"y2", quad.mB.y} };
    }

    inline void from_json(const nlohmann::json& json, math::Quad& quad)
    {
        quad.mA.x = json["x1"].get<float>();
        quad.mA.y = json["y1"].get<float>();
        quad.mB.x = json["x2"].get<float>();
        quad.mB.y = json["y2"].get<float>();
    }

    // Interval
    inline void to_json(nlohmann::json& j, const math::Interval& interval)
    {
        j = nlohmann::json{ {"min", interval.min}, {"max", interval.max} };
    }

    inline void from_json(const nlohmann::json& json, math::Interval& interval)
    {
        interval.min = json["min"].get<float>();
        interval.max = json["max"].get<float>();
    }

    // ValueSpread
    inline void to_json(nlohmann::json& j, const math::ValueSpread& value_spread)
    {
        j = nlohmann::json{ {"value", value_spread.value}, {"spread", value_spread.spread} };
    }

    inline void from_json(const nlohmann::json& json, math::ValueSpread& value_spread)
    {
        value_spread.value = json["value"].get<float>();
        value_spread.spread = json["spread"].get<math::Interval>();
    }
}

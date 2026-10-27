
#pragma once

#include "Math/Interval.h"
#include "Math/Vector.h"
#include "Rendering/Color.h"
#include "EntityTypes.h"

#include <variant>
#include <string>
#include <vector>

namespace mono
{
    enum class EventType : int
    {
        Global,
        Local,
        Entity
    };

    enum class EventDirection : int
    {
        Input,
        Output
    };

    struct Event
    {
        EventType type;
        EventDirection direction;
        std::string text;
    };

    using Variant = std::variant<
        bool,
        int,
        uint32_t,
        float,
        math::Vector,
        mono::Color::RGBA,
        std::string,
        std::vector<math::Vector>,
        math::Interval,
        math::ValueSpread,
        mono::Color::Gradient<4>,
        mono::Event>;
}

struct Attribute
{
    uint32_t id = 0;
    mono::Variant value;
};

//
// This is to make sure that types stay in the same place since we depend on that when serializing
//

enum VariantTypeIndex
{
    BOOL,
    INT,
    UINT,
    FLOAT,
    POINT,
    COLOR,
    STRING,
    POLYGON,
    INTERVAL,
    VALUE_SPREAD,
    GRADIENT_4,
    EVENT
};

static_assert(std::is_same_v<bool,                      std::variant_alternative_t<VariantTypeIndex::BOOL, mono::Variant>>);
static_assert(std::is_same_v<int,                       std::variant_alternative_t<VariantTypeIndex::INT, mono::Variant>>);
static_assert(std::is_same_v<uint32_t,                  std::variant_alternative_t<VariantTypeIndex::UINT, mono::Variant>>);
static_assert(std::is_same_v<float,                     std::variant_alternative_t<VariantTypeIndex::FLOAT, mono::Variant>>);
static_assert(std::is_same_v<math::Vector,              std::variant_alternative_t<VariantTypeIndex::POINT, mono::Variant>>);
static_assert(std::is_same_v<mono::Color::RGBA,         std::variant_alternative_t<VariantTypeIndex::COLOR, mono::Variant>>);
static_assert(std::is_same_v<std::string,               std::variant_alternative_t<VariantTypeIndex::STRING, mono::Variant>>);
static_assert(std::is_same_v<std::vector<math::Vector>, std::variant_alternative_t<VariantTypeIndex::POLYGON, mono::Variant>>);
static_assert(std::is_same_v<math::Interval,            std::variant_alternative_t<VariantTypeIndex::INTERVAL, mono::Variant>>);
static_assert(std::is_same_v<math::ValueSpread,         std::variant_alternative_t<VariantTypeIndex::VALUE_SPREAD, mono::Variant>>);
static_assert(std::is_same_v<mono::Color::Gradient<4>,  std::variant_alternative_t<VariantTypeIndex::GRADIENT_4, mono::Variant>>);
static_assert(std::is_same_v<mono::Event,               std::variant_alternative_t<VariantTypeIndex::EVENT, mono::Variant>>);

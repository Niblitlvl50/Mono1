
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

    struct Event
    {
        EventType type;
        std::string text;
    };
}

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

struct Attribute
{
    uint32_t id = 0;
    Variant value;
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

static_assert(std::is_same_v<bool,                      std::variant_alternative_t<VariantTypeIndex::BOOL, Variant>>);
static_assert(std::is_same_v<int,                       std::variant_alternative_t<VariantTypeIndex::INT, Variant>>);
static_assert(std::is_same_v<uint32_t,                  std::variant_alternative_t<VariantTypeIndex::UINT, Variant>>);
static_assert(std::is_same_v<float,                     std::variant_alternative_t<VariantTypeIndex::FLOAT, Variant>>);
static_assert(std::is_same_v<math::Vector,              std::variant_alternative_t<VariantTypeIndex::POINT, Variant>>);
static_assert(std::is_same_v<mono::Color::RGBA,         std::variant_alternative_t<VariantTypeIndex::COLOR, Variant>>);
static_assert(std::is_same_v<std::string,               std::variant_alternative_t<VariantTypeIndex::STRING, Variant>>);
static_assert(std::is_same_v<std::vector<math::Vector>, std::variant_alternative_t<VariantTypeIndex::POLYGON, Variant>>);
static_assert(std::is_same_v<math::Interval,            std::variant_alternative_t<VariantTypeIndex::INTERVAL, Variant>>);
static_assert(std::is_same_v<math::ValueSpread,         std::variant_alternative_t<VariantTypeIndex::VALUE_SPREAD, Variant>>);
static_assert(std::is_same_v<mono::Color::Gradient<4>,  std::variant_alternative_t<VariantTypeIndex::GRADIENT_4, Variant>>);
static_assert(std::is_same_v<mono::Event,               std::variant_alternative_t<VariantTypeIndex::EVENT, Variant>>);

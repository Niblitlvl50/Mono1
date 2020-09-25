
#pragma once

#include "Math/Vector.h"
#include "Rendering/Color.h"
#include <assert.h>
#include <algorithm>
#include <cstring>

constexpr int VariantStringMaxLength = 32;

class Variant
{
public:

    Variant()
    { }

    explicit Variant(bool value)
    {
        *this = value;
    }

    explicit Variant(int value)
    {
        *this = value;
    }

    explicit Variant(uint32_t value)
    {
        *this = value;
    }

    explicit Variant(float value)
    {
        *this = value;
    }

    explicit Variant(const char* string)
    {
        *this = string;
    }

    explicit Variant(const math::Vector& point)
    {
        *this = point;
    }

    explicit Variant(const mono::Color::RGBA& color)
    {
        *this = color;
    }

    operator bool() const
    {
        assert(type == Type::BOOL);
        return int_value;
    }

    operator int() const
    {
        assert(type == Type::INT);
        return int_value;
    }

    operator uint32_t() const
    {
        assert(tyep == Type::UINT);
        return uint_value;
    }

    operator float() const
    {
        assert(type == Type::FLOAT);
        return float_value;
    }

    operator const char*() const
    {
        assert(type == Type::STRING);
        return string_value;
    }

    operator math::Vector() const
    {
        assert(type == Type::POINT);
        return point_value;
    }

    operator mono::Color::RGBA() const
    {
        assert(type == Type::COLOR);
        return color_value;
    }

    Variant& operator = (const bool value)
    {
        type = Type::BOOL;
        int_value = value;
        return *this;
    }

    Variant& operator = (const int value)
    {
        type = Type::INT;
        int_value = value;
        return *this;
    }

    Variant& operator = (const uint32_t value)
    {
        type = Type::UINT;
        uint_value = value;
        return *this;
    }

    Variant& operator = (const float value)
    {
        type = Type::FLOAT;
        float_value = value;
        return *this;
    }

    Variant& operator = (const char* string)
    {
        type = Type::STRING;

        const size_t length = std::strlen(string);
        const size_t data_length = std::min(length, size_t(VariantStringMaxLength - 1));

        std::memcpy(string_value, string, data_length);
        string_value[data_length] = '\0';

        return *this;
    }

    Variant& operator = (const math::Vector& value)
    {
        type = Type::POINT;
        point_value = value;
        return *this;
    }

    Variant& operator = (const mono::Color::RGBA& color)
    {
        type = Type::COLOR;
        color_value = color;
        return *this;
    }

    enum class Type : short
    {
        NONE,
        BOOL,
        INT,
        FLOAT,
        STRING,
        POINT,
        COLOR,
        UINT,
    };

    Type type = Type::NONE;

    union
    {
        int int_value;
        uint32_t uint_value;
        float float_value;
        char string_value[VariantStringMaxLength];
        math::Vector point_value;
        mono::Color::RGBA color_value;
    };
};

struct Attribute
{
    uint32_t id = 0;
    Variant attribute;
};

static_assert(sizeof(Variant) == 36, "Variant size is too big!");
static_assert(sizeof(Attribute) == 40, "Attribute size is too big!");

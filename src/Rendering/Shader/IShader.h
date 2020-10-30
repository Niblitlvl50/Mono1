
#pragma once

#include "Math/MathFwd.h"
#include "Rendering/RenderFwd.h"
#include <cstdint>

namespace mono
{
    class IShader
    {
    public:

        virtual ~IShader() = default;
        virtual void Use() = 0;
        virtual uint32_t GetShaderId() const = 0;
        virtual int GetAttributeLocation(const char* attribute_name) = 0;
        virtual void SetValue(const char* property_name, int value) = 0;
        virtual void SetValue(const char* property_name, float value) = 0;
        virtual void SetValue(const char* property_name, const math::Vector& vector) = 0;
        virtual void SetValue(const char* property_name, const math::Matrix& transform) = 0;
        virtual void SetValue(const char* property_name, const mono::Color::RGBA& color) = 0;
        virtual void SetTime(float total_time, float delta_time) = 0;
        virtual void SetProjectionAndModelView(const math::Matrix& projection, const math::Matrix& model_view) = 0;
    };
}

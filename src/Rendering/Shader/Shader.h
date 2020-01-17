
#pragma once

#include "IShader.h"
#include <cstdint>
#include <unordered_map>

namespace mono
{
    class Shader : public IShader
    {
    public:

        Shader(const char* vertex_source, const char* fragment_source);
        ~Shader();

        void Use() override;
        uint32_t GetShaderId() const override;
        int GetAttributeLocation(const char* attribute_name) override;
        void SetValue(const char* property_name, int value) override;
        void SetValue(const char* property_name, float value) override;
        void SetValue(const char* property_name, const math::Vector& vector) override;
        void SetValue(const char* property_name, const math::Matrix& transform) override;
        void SetValue(const char* property_name, const mono::Color::RGBA& color) override;
        void SetProjectionAndModelView(const math::Matrix& projection, const math::Matrix& model_view) override;

    private:

        uint32_t m_program;
        std::unordered_map<uint32_t, int> m_location_cache;
    };
}

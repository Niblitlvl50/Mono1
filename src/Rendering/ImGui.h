
#pragma once

#include "Math/Vector.h"
#include <unordered_map>

namespace mono
{
    class IRenderer;
    class IShader;

    struct ImGuiContext
    {
        math::Vector window_size;
        std::unique_ptr<mono::IShader> shader;
    };

    void InitializeImGui(ImGuiContext& imgui_context);
    void DestroyImGui();
    void DrawImGui(mono::ImGuiContext& imgui_context, mono::IRenderer& renderer);

    void SetImGuiConfig(const char* config_file);
    uint32_t LoadImGuiTexture(const char* texture_file);
}

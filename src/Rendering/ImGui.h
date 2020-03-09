
#pragma once

#include "Math/Vector.h"
#include "Rendering/RenderPtrFwd.h"
#include <unordered_map>
#include <memory>

namespace mono
{
    struct ImGuiContext
    {
        math::Vector window_size;
        std::unique_ptr<class IShader> shader;
    };

    void InitializeImGui(ImGuiContext& imgui_context);
    void DestroyImGui();
    void DrawImGui(mono::ImGuiContext& imgui_context, class IRenderer& renderer);

    void SetImGuiConfig(const char* config_file);
    uint32_t LoadImGuiTexture(const char* texture_file);
    void LoadImGuiTexture(mono::ITexturePtr texture);
}


#pragma once

#include "IGameSystem.h"
#include "Util/ActiveVector.h"
#include <stddef.h>

namespace System
{
    class IWindow;
}

namespace mono
{
    struct RenderInitParams
    {
        float pixels_per_meter = 32.0f;
        const char* light_mask_texture = nullptr;
        const char* sprite_shadow_texture = nullptr;
        const char* imgui_ini = nullptr;
        System::IWindow* window = nullptr;
    };

    void InitializeRender(const RenderInitParams& init_params);
    void ShutdownRender();

    float PixelsPerMeter();
    float GetWindowAspect();

    const char* LightMaskTexture();
    const char* SpriteShadowTexture();

    void LoadCustomTextureFactory(const class ITextureFactory* texture_factory);

    const class ISpriteFactory* GetSpriteFactory();
    const class ITextureFactory* GetTextureFactory();

    struct LayerComponent
    {
        int layer;
    };

    class RenderSystem : public mono::IGameSystem
    {
    public:

        RenderSystem(uint32_t n);

        const char* Name() const override;
        void Update(const mono::UpdateContext& update_context) override;

        void AllocateLayer(uint32_t entity_id);
        void ReleaseLayer(uint32_t entity_id);
        void UpdateLayer(uint32_t entity_id, int layer);

        int GetRenderLayerForEntity(uint32_t entity_id);

        mono::ActiveVector<LayerComponent> m_layers;
    };
}

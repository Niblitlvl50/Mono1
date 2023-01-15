
#pragma once

#include "IGameSystem.h"
#include "Util/ActiveVector.h"

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

    struct LayerComponent
    {
        int layer;
        float sort_offset;
    };

    class RenderSystem : public mono::IGameSystem
    {
    public:

        RenderSystem(uint32_t n, const RenderInitParams& init_params);
        ~RenderSystem();

        const char* Name() const override;
        void Update(const mono::UpdateContext& update_context) override;

        void AllocateLayer(uint32_t entity_id);
        void ReleaseLayer(uint32_t entity_id);
        void UpdateLayer(uint32_t entity_id, int new_layer, float new_sort_offset);

        int GetRenderLayerOrDefault(uint32_t entity_id) const;
        float GetRenderSortOffsetOrDefault(uint32_t entity_id) const;

        static float PixelsPerMeter();
        static float GetWindowAspect();
        static const char* LightMaskTexture();
        static const char* SpriteShadowTexture();
        static void LoadCustomTextureFactory(const class ITextureFactory* texture_factory);
        static const class ISpriteFactory* GetSpriteFactory();
        static const class ITextureFactory* GetTextureFactory();

    private:

        mono::ActiveVector<LayerComponent> m_layers;

        static float s_pixels_per_meter;
        static const char* s_light_mask_texture;
        static const char* s_sprite_shadow_texture;
        static const System::IWindow* s_window;
        static const mono::ISpriteFactory* s_sprite_factory;
        static const mono::ITextureFactory* s_texture_factory;
    };
}

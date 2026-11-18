
#pragma once

#include "Math/Quad.h"
#include "Rendering/RenderFwd.h"

namespace mono
{
    // The base layer plus four painted ones, one for each channel of the splat map.
    constexpr int GROUND_PAINT_LAYERS = 5;

    struct GroundPaintDrawParams
    {
        // RGBA weights of layer 1 to 4 over splat_rect, what's left over is the base layer.
        const ITexture* splat_map = nullptr;
        // A layer without a texture is see through.
        const ITexture* layer_textures[GROUND_PAINT_LAYERS] = { };

        // One means one texel of a layer texture to one pixel on screen.
        float layer_scales[GROUND_PAINT_LAYERS] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };

        math::Quad splat_rect;

        // How ragged the edges between layers are, zero makes them follow the splat map exactly.
        float edge_roughness = 0.3f;

        // Blends the layers by their weights instead of showing the strongest one.
        bool blend_layers = false;
    };
}

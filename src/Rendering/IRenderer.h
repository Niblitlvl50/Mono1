
#pragma once

#include "RenderPtrFwd.h"
#include "BlendMode.h"
#include "Math/MathFwd.h"

#include <vector>
#include <stdlib.h>

namespace mono
{
    class IRenderBuffer;

    class IRenderer
    {
    public:

        virtual ~IRenderer() = default;
        virtual void AddDrawable(const IDrawable* drawable) = 0;

        virtual void DrawText(int font_id, const char* text, const math::Vector& pos, bool center, const mono::Color::RGBA& color) const = 0;
        virtual void DrawSprite(const ISprite& sprite) const = 0;
        virtual void DrawSprite(const ISprite& sprite, const math::Vector& offset) const = 0;
        virtual void DrawSprite(
            const math::Quad& sprite_coords, const math::Vector& size, const math::Vector& offset, const ITexturePtr& texture) const = 0;
        virtual void DrawPoints(const std::vector<math::Vector>& points, const mono::Color::RGBA& color, float size) const = 0;
        virtual void DrawLines(const std::vector<math::Vector>& linePoints, const mono::Color::RGBA& color, float width) const = 0;
        virtual void DrawPolyline(const std::vector<math::Vector>& linePoints, const mono::Color::RGBA& color, float width) const = 0;
        virtual void DrawClosedPolyline(const std::vector<math::Vector>& linePoints, const mono::Color::RGBA& color, float width) const = 0;
        virtual void DrawQuad(const math::Quad& quad, const mono::Color::RGBA& color, float width) const = 0;
        virtual void DrawFilledQuad(const math::Quad& quad, const mono::Color::RGBA& color) const = 0;
        virtual void DrawCircle(const math::Vector& pos, float radie, int segments, float lineWidth, const mono::Color::RGBA& color) const = 0;

        virtual void DrawGeometry(const std::vector<math::Vector>& vertices,
                                  const std::vector<math::Vector>& texture_coordinates,
                                  const std::vector<uint16_t>& indices,
                                  const ITexturePtr& texture) = 0;

        virtual void DrawGeometry(const IRenderBuffer* vertices,
                                  const IRenderBuffer* texture_coordinates,
                                  size_t offset,
                                  size_t count,
                                  const ITexturePtr& texture) = 0;

        virtual void DrawParticlePoints(
            const IRenderBuffer* position,
            const IRenderBuffer* rotation,
            const IRenderBuffer* color,
            const IRenderBuffer* point_size,
            const ITexturePtr& texture,
            BlendMode blend_mode,
            size_t count) = 0;

        virtual void DrawPolyline(
            const IRenderBuffer* vertices, const IRenderBuffer* colors, size_t offset, size_t count) = 0;

        virtual void DrawTrianges(
            const IRenderBuffer* vertices, const IRenderBuffer* colors, const IRenderBuffer* indices, size_t count) const = 0;

        virtual void UseShader(IShader* shader) const = 0;
        virtual void UseTexture(const ITexturePtr& texture) const = 0;
        virtual void ClearTexture() = 0;

        virtual void PushGlobalTransform() = 0;

        virtual void PushNewTransform(const math::Matrix& transform) = 0;
        virtual const math::Matrix& GetCurrentTransform() const = 0;

        virtual void PushNewProjection(const math::Matrix& projection) = 0;
        virtual const math::Matrix& GetCurrentProjection() const = 0;

        virtual const math::Quad& GetViewport() const = 0;
        virtual bool Cull(const math::Quad& world_bb) const = 0;
    };
}

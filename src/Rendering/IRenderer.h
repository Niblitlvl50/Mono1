
#pragma once

#include "RenderFwd.h"
#include "BlendMode.h"
#include "Math/MathFwd.h"

#include <vector>
#include <cstdint>

namespace mono
{
    class IRenderer
    {
    public:

        virtual ~IRenderer() = default;
        virtual void AddDrawable(const IDrawable* drawable) = 0;

        virtual void RenderText(int font_id, const char* text, const math::Vector& pos, bool center, const mono::Color::RGBA& color) const = 0;
        virtual void DrawSprite(const ISprite& sprite) const = 0;
        virtual void DrawSprite(
            const math::Vector& uv_upper_left,
            const math::Vector& uv_lower_right,
            const math::Vector& size,
            const math::Vector& offset,
            const ITexture* texture) const = 0;

        virtual void DrawPoints(const std::vector<math::Vector>& points, const mono::Color::RGBA& color, float size) const = 0;
        virtual void DrawLines(const std::vector<math::Vector>& line_points, const mono::Color::RGBA& color, float width) const = 0;
        virtual void DrawPolyline(const std::vector<math::Vector>& line_points, const mono::Color::RGBA& color, float width) const = 0;
        virtual void DrawClosedPolyline(const std::vector<math::Vector>& line_points, const mono::Color::RGBA& color, float width) const = 0;
        virtual void DrawQuad(const math::Quad& quad, const mono::Color::RGBA& color, float width) const = 0;
        virtual void DrawFilledQuad(const math::Quad& quad, const mono::Color::RGBA& color) const = 0;
        virtual void DrawCircle(const math::Vector& pos, float radie, int segments, float line_width, const mono::Color::RGBA& color) const = 0;
        virtual void DrawFilledCircle(const math::Vector& pos, const math::Vector& size, int segments, const mono::Color::RGBA& color) const = 0;

        virtual void DrawGeometry(
            const std::vector<math::Vector>& vertices,
            const std::vector<math::Vector>& texture_coordinates,
            const std::vector<uint16_t>& indices,
            const ITexture* texture) = 0;

        virtual void DrawGeometry(
            const IRenderBuffer* vertices,
            const IRenderBuffer* texture_coordinates,
            size_t offset,
            size_t count,
            const ITexture* texture) = 0;

        virtual void DrawParticlePoints(
            const IRenderBuffer* position,
            const IRenderBuffer* rotation,
            const IRenderBuffer* color,
            const IRenderBuffer* point_size,
            const ITexture* texture,
            BlendMode blend_mode,
            size_t count) = 0;

        virtual void DrawPolyline(
            const IRenderBuffer* vertices, const IRenderBuffer* colors, size_t offset, size_t count) = 0;

        virtual void DrawTrianges(
            const IRenderBuffer* vertices, const IRenderBuffer* colors, const IElementBuffer* indices, size_t count) const = 0;

        virtual void SetClearColor(const mono::Color::RGBA& color) = 0;

        virtual void UseShader(IShader* shader) const = 0;
        virtual void UseTexture(const ITexture* texture) const = 0;
        virtual void ClearTexture() = 0;

        virtual const math::Matrix& GetTransform() const = 0;
        virtual void PushNewTransform(const math::Matrix& transform) = 0;
        virtual void PopTransform() = 0;

        virtual void PushNewProjection(const math::Matrix& projection) = 0;
        virtual void PopProjection() = 0;

        virtual void PushNewViewTransform(const math::Matrix& transform) = 0;
        virtual void PopViewTransform() = 0;

        virtual const math::Quad& GetViewport() const = 0;
        virtual bool Cull(const math::Quad& world_bb) const = 0;

        virtual uint32_t GetDeltaTimeMS() const = 0;
        virtual uint32_t GetTimestamp() const = 0;

        virtual IShader* GetSpriteShader() = 0;
        virtual IShader* GetScreenShader() = 0;
    };

    using PushTransformFunc = void (mono::IRenderer::*)(const math::Matrix& transform);
    using PopTransformFunc = void (mono::IRenderer::*)();

    class ScopedTransform
    {
    public:

        ScopedTransform(const math::Matrix& transform, IRenderer* renderer, PushTransformFunc push, PopTransformFunc pop)
            : m_renderer(renderer)
            , m_pop_func(pop)
        {
            (m_renderer->*push)(transform);
        }

        ~ScopedTransform()
        {
            (m_renderer->*m_pop_func)();
        }

        ScopedTransform(ScopedTransform const&) = delete;
        ScopedTransform& operator=(ScopedTransform const&) = delete;

        IRenderer* m_renderer;
        PopTransformFunc m_pop_func;
    };

    inline ScopedTransform MakeTransformScope(const math::Matrix& transform, IRenderer* renderer)
    {
        return ScopedTransform(transform, renderer, &mono::IRenderer::PushNewTransform, &mono::IRenderer::PopTransform);
    }

    inline ScopedTransform MakeProjectionScope(const math::Matrix& transform, IRenderer* renderer)
    {
        return ScopedTransform(transform, renderer, &mono::IRenderer::PushNewProjection, &mono::IRenderer::PopProjection);
    }

    inline ScopedTransform MakeViewTransformScope(const math::Matrix& transform, IRenderer* renderer)
    {
        return ScopedTransform(transform, renderer, &mono::IRenderer::PushNewViewTransform, &mono::IRenderer::PopViewTransform);
    }
}

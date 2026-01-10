
#pragma once

#include "RenderFwd.h"
#include "BlendMode.h"
#include "Rendering/Text/TextFlags.h"
#include "Math/MathFwd.h"

#include <vector>
#include <cstdint>

namespace mono
{
    enum RenderPass
    {
        GENERAL,
        POST_LIGHTING,
        N_RENDER_PASS
    };

    enum class CullResult
    {
        IN_VIEW,
        OUTSIDE_VIEW
    };

    class IRenderer
    {
    public:

        virtual ~IRenderer() = default;
        virtual void AddDrawable(const IDrawable* drawable, RenderPass render_pass = RenderPass::GENERAL) = 0;

        virtual void RenderText(
            int font_id,
            const char* text,
            const mono::Color::RGBA& color,
            mono::FontCentering center_flags) const = 0;

        virtual void DrawSprite(
            const ISprite* sprite,
            const IRenderBuffer* vertices,
            const IRenderBuffer* offsets,
            const IRenderBuffer* uv_coordinates,
            const IRenderBuffer* uv_coordinates_flipped,
            const IRenderBuffer* height_values,
            const IElementBuffer* indices,
            const ITexture* texture,
            uint32_t buffer_offset) const = 0;

        virtual void DrawSprite(
            const ISprite* sprite,
            const SpriteDrawBuffers* buffers,
            const IElementBuffer* indices,
            uint32_t buffer_offset) const = 0;

        virtual void DrawSpriteOutline(
            const ISprite* sprite,
            const SpriteDrawBuffers* buffers,
            const IElementBuffer* indices,
            uint32_t buffer_offset) const = 0;

        virtual void DrawPoints(const std::vector<math::Vector>& points, const mono::Color::RGBA& color, float point_size) const = 0;
        virtual void DrawPoints(const std::vector<math::Vector>& points, const std::vector<mono::Color::RGBA>& colors, float point_size) const = 0;
        virtual void DrawLines(const std::vector<math::Vector>& line_points, const mono::Color::RGBA& color, float width) const = 0;
        virtual void DrawPolyline(const std::vector<math::Vector>& line_points, const mono::Color::RGBA& color, float width) const = 0;
        virtual void DrawClosedPolyline(const std::vector<math::Vector>& line_points, const mono::Color::RGBA& color, float width) const = 0;
        virtual void DrawQuad(const math::Quad& quad, const mono::Color::RGBA& color, float width) const = 0;
        virtual void DrawFilledQuad(const math::Quad& quad, const mono::Color::RGBA& color) const = 0;
        virtual void DrawCircle(const math::Vector& pos, float radie, int segments, float line_width, const mono::Color::RGBA& color) const = 0;
        virtual void DrawFilledCircle(const math::Vector& size, int segments, const mono::Color::RGBA& color) const = 0;

        virtual void DrawGeometry(
            const IRenderBuffer* vertices,
            const IRenderBuffer* texture_coordinates,
            const IElementBuffer* indices,
            const ITexture* texture,
            const mono::Color::RGBA& shade,
            bool blur,
            uint32_t count) const = 0;

        virtual void DrawGeometry(
            const IRenderBuffer* vertices,
            const IRenderBuffer* uv_coordinates,
            const IRenderBuffer* vertex_colors,
            const IElementBuffer* indices,
            const ITexture* texture,
            bool blur,
            uint32_t count) const = 0;

        virtual void DrawParticlePoints(
            const IRenderBuffer* position,
            const IRenderBuffer* rotation,
            const IRenderBuffer* color,
            const IRenderBuffer* point_size,
            const ITexture* texture,
            BlendMode blend_mode,
            uint32_t count) = 0;

        virtual void DrawFog(
            const IRenderBuffer* vertices, const IElementBuffer* indices, const ITexture* texture) = 0;

        virtual void DrawPoints(
            const IRenderBuffer* vertices, const IRenderBuffer* colors, float point_size, uint32_t offset, uint32_t count) const = 0;

        virtual void DrawLines(
            const IRenderBuffer* vertices, const IRenderBuffer* colors, uint32_t offset, uint32_t count) = 0;
        virtual void DrawLines(
            const IRenderBuffer* vertices, const IRenderBuffer* colors, const IElementBuffer* indices, uint32_t offset, uint32_t count) = 0;

        virtual void DrawPolyline(
            const IRenderBuffer* vertices, const IRenderBuffer* colors, uint32_t offset, uint32_t count) = 0;
        virtual void DrawPolyline(
            const IRenderBuffer* vertices, const IRenderBuffer* colors, const IElementBuffer* indices, uint32_t offset, uint32_t count) = 0;

        virtual void DrawTrianges(
            const IRenderBuffer* vertices, const IRenderBuffer* colors, const IElementBuffer* indices, uint32_t offset, uint32_t count) const = 0;
        virtual void DrawAnnotatedTrianges(
            const IRenderBuffer* vertices,
            const IRenderBuffer* annotations,
            const IElementBuffer* indices,
            const ITexture* texture,
            const mono::Color::RGBA& shade,
            uint32_t offset,
            uint32_t count) const = 0;

        virtual void AddLight(const math::Vector& world_position, float radius, const mono::Color::RGBA& shade) = 0;

        virtual void SetClearColor(const mono::Color::RGBA& color) = 0;
        virtual void SetAmbientShade(const mono::Color::RGBA& ambient_shade) = 0;
        virtual void ToggleLighting() = 0;

        virtual void SetScreenFadeAlpha(float alpha) = 0;

        virtual const math::Matrix& GetTransform() const = 0;
        virtual void PushNewTransform(const math::Matrix& transform) = 0;
        virtual void PopTransform() = 0;

        virtual void PushNewProjection(const math::Matrix& projection) = 0;
        virtual void PopProjection() = 0;

        virtual void PushNewViewTransform(const math::Matrix& transform) = 0;
        virtual void PopViewTransform() = 0;

        virtual const math::Quad& GetViewport() const = 0;
        virtual CullResult Cull(const math::Quad& world_bb) const = 0;

        virtual float GetDeltaTime() const = 0;
        virtual uint32_t GetTimestamp() const = 0;
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

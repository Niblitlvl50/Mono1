
#pragma once

#include "IRenderer.h"
#include "Rendering/Texture/ITextureFactory.h"

#include "Color.h"
#include "Math/Vector.h"
#include "Math/Quad.h"
#include "Math/Matrix.h"

#include <stack>
#include <vector>
#include <memory>

#include "sokol/sokol_gfx.h"

namespace mono
{
    class RendererSokol : public mono::IRenderer
    {
    public:

        RendererSokol();
        ~RendererSokol();

        void SetWindowSize(const math::Vector& window_size);
        void SetDrawableSize(const math::Vector& drawable_size);
        void SetViewport(const math::Quad& viewport);
        void SetDeltaAndTimestamp(uint32_t delta_ms, uint32_t timestamp);

        void DrawFrame();

        void AddDrawable(const IDrawable* drawable) override;

        void RenderText(
            int font_id,
            const char* text,
            const math::Vector& pos,
            bool center,
            const mono::Color::RGBA& color) const override;
        void DrawSprite(const ISprite& sprite) const override;
        void DrawSprite(
            const math::Vector& uv_upper_left,
            const math::Vector& uv_lower_right,
            const math::Vector& size,
            const math::Vector& offset,
            const ITexture* texture) const override;

        void DrawPoints(const std::vector<math::Vector>& points, const mono::Color::RGBA& color, float point_size) const override;
        void DrawLines(const std::vector<math::Vector>& line_points, const mono::Color::RGBA& color, float width) const override;
        void DrawPolyline(const std::vector<math::Vector>& line_points, const mono::Color::RGBA& color, float width) const override;
        void DrawClosedPolyline(const std::vector<math::Vector>& line_points, const mono::Color::RGBA& color, float width) const override;
        void DrawQuad(const math::Quad& quad, const mono::Color::RGBA& color, float width) const override;
        void DrawFilledQuad(const math::Quad& quad, const mono::Color::RGBA& color) const override;
        void DrawCircle(const math::Vector& pos, float radie, int segments, float line_width, const mono::Color::RGBA& color) const override;
        void DrawFilledCircle(const math::Vector& pos, const math::Vector& size, int segments, const mono::Color::RGBA& color) const override;

        void DrawGeometry(
            const IRenderBuffer* vertices,
            const IRenderBuffer* texture_coordinates,
            const IElementBuffer* indices,
            const ITexture* texture,
            uint32_t count) override;

        void DrawParticlePoints(
            const IRenderBuffer* position,
            const IRenderBuffer* rotation,
            const IRenderBuffer* color,
            const IRenderBuffer* point_size,
            const ITexture* texture,
            BlendMode blend_mode,
            uint32_t count) override;

        void DrawPolyline(
            const IRenderBuffer* vertices, const IRenderBuffer* colors, uint32_t offset, uint32_t count) override;

        void DrawTrianges(
            const IRenderBuffer* vertices, const IRenderBuffer* colors, const IElementBuffer* indices, uint32_t count) const override;

        void SetClearColor(const mono::Color::RGBA& color) override;

        const math::Matrix& GetTransform() const override;
        void PushNewTransform(const math::Matrix& transform) override;
        void PopTransform() override;

        void PushNewProjection(const math::Matrix& projection) override;
        void PopProjection() override;

        void PushNewViewTransform(const math::Matrix& transform) override;
        void PopViewTransform() override;

        const math::Quad& GetViewport() const override;
        bool Cull(const math::Quad& world_bb) const override;

        uint32_t GetDeltaTimeMS() const override;
        uint32_t GetTimestamp() const override;

    private:

        struct OffscreenPassData
        {
            math::Vector image_size;
            mono::ITexturePtr offscreen_texture;
            sg_pass pass_handle;
        } m_offscreen_pass;

        void MakeOrUpdateOffscreenPass(OffscreenPassData& offscreen_pass) const;
        void PrepareDraw();
        void EndDraw();

        math::Vector m_window_size;
        math::Vector m_drawable_size;
        math::Quad m_viewport;

        mono::Color::RGBA m_clear_color;

        std::stack<math::Matrix> m_projection_stack;
        std::stack<math::Matrix> m_model_stack;
        std::stack<math::Matrix> m_view_stack;

        std::unique_ptr<IPipeline> m_color_points_pipeline;
        std::unique_ptr<IPipeline> m_color_lines_pipeline;
        std::unique_ptr<IPipeline> m_color_line_strip_pipeline;
        std::unique_ptr<IPipeline> m_color_triangles_pipeline;
        std::unique_ptr<IPipeline> m_particle_pipeline;
        std::unique_ptr<IPipeline> m_texture_pipeline;
        std::unique_ptr<IPipeline> m_sprite_pipeline;
        std::unique_ptr<IPipeline> m_screen_pipeline;

        std::unique_ptr<IRenderBuffer> m_screen_vertices;
        std::unique_ptr<IRenderBuffer> m_screen_uv;
        std::unique_ptr<IElementBuffer> m_screen_indices;

        std::unique_ptr<IRenderBuffer> m_temp_vertices;
        std::unique_ptr<IRenderBuffer> m_temp_uv_coords;
        std::unique_ptr<IRenderBuffer> m_temp_colors;
        std::unique_ptr<IRenderBuffer> m_temp_heights;
        std::unique_ptr<IElementBuffer> m_temp_indices;

        uint32_t m_delta_time_ms = 0;
        uint32_t m_timestamp = 0;

        std::vector<const IDrawable*> m_drawables;
    };
}

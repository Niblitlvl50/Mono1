
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
        void SetDeltaAndTimestamp(float delta_s, uint32_t timestamp);

        void DrawFrame();

        void AddDrawable(const IDrawable* drawable, RenderPass render_pass) override;

        void RenderText(
            int font_id,
            const char* text,
            const mono::Color::RGBA& color,
            mono::FontCentering center_flags) const override;

        void DrawSprite(
            const ISprite* sprite,
            const IRenderBuffer* vertices,
            const IRenderBuffer* offsets,
            const IRenderBuffer* uv_coordinates,
            const IRenderBuffer* uv_coordinates_flipped,
            const IRenderBuffer* height_values,
            const IElementBuffer* indices,
            const ITexture* texture,
            uint32_t buffer_offset) const override;

        void DrawSprite(
            const ISprite* sprite,
            const SpriteDrawBuffers* buffers,
            const IElementBuffer* indices,
            uint32_t buffer_offset) const override;

        void DrawPoints(const std::vector<math::Vector>& points, const mono::Color::RGBA& color, float point_size) const override;
        void DrawPoints(const std::vector<math::Vector>& points, const std::vector<mono::Color::RGBA>& colors, float point_size) const override;
        void DrawLines(const std::vector<math::Vector>& line_points, const mono::Color::RGBA& color, float width) const override;
        void DrawPolyline(const std::vector<math::Vector>& line_points, const mono::Color::RGBA& color, float width) const override;
        void DrawClosedPolyline(const std::vector<math::Vector>& line_points, const mono::Color::RGBA& color, float width) const override;
        void DrawQuad(const math::Quad& quad, const mono::Color::RGBA& color, float width) const override;
        void DrawFilledQuad(const math::Quad& quad, const mono::Color::RGBA& color) const override;
        void DrawCircle(const math::Vector& pos, float radie, int segments, float line_width, const mono::Color::RGBA& color) const override;
        void DrawFilledCircle(const math::Vector& pos, const math::Vector& size, int segments, const mono::Color::RGBA& color) const override;

        void DrawGeometry(
            const IRenderBuffer* vertices,
            const IRenderBuffer* uv_coordinates,
            const IElementBuffer* indices,
            const ITexture* texture,
            const mono::Color::RGBA& shade,
            bool blur,
            uint32_t count) const override;

        void DrawGeometry(
            const IRenderBuffer* vertices,
            const IRenderBuffer* uv_coordinates,
            const IRenderBuffer* vertex_colors,
            const IElementBuffer* indices,
            const ITexture* texture,
            bool blur,
            uint32_t count) const override;

        void DrawParticlePoints(
            const IRenderBuffer* position,
            const IRenderBuffer* rotation,
            const IRenderBuffer* color,
            const IRenderBuffer* point_size,
            const ITexture* texture,
            BlendMode blend_mode,
            uint32_t count) override;

        void DrawFog(
            const IRenderBuffer* vertices, const IElementBuffer* indices, const ITexture* texture) override;

        void DrawPoints(
            const IRenderBuffer* vertices, const IRenderBuffer* colors, float point_size, uint32_t offset, uint32_t count) override;

        void DrawLines(
            const IRenderBuffer* vertices, const IRenderBuffer* colors, uint32_t offset, uint32_t count) override;
        void DrawLines(
            const IRenderBuffer* vertices, const IRenderBuffer* colors, const IElementBuffer* indices, uint32_t offset, uint32_t count) override;

        void DrawPolyline(
            const IRenderBuffer* vertices, const IRenderBuffer* colors, uint32_t offset, uint32_t count) override;
        void DrawPolyline(
            const IRenderBuffer* vertices, const IRenderBuffer* colors, const IElementBuffer* indices, uint32_t offset, uint32_t count) override;

        void DrawTrianges(
            const IRenderBuffer* vertices, const IRenderBuffer* colors, const IElementBuffer* indices, uint32_t offset, uint32_t count) const override;
        void DrawAnnotatedTrianges(
            const IRenderBuffer* vertices,
            const IRenderBuffer* annotations,
            const IElementBuffer* indices,
            const ITexture* texture,
            const mono::Color::RGBA& shade,
            uint32_t offset,
            uint32_t count) const override;

        void AddLight(const math::Vector& world_position, float radius, const mono::Color::RGBA& shade) override;

        void SetClearColor(const mono::Color::RGBA& color) override;
        void SetAmbientShade(const mono::Color::RGBA& ambient_shade) override;
        void ToggleLighting() override;

        void SetScreenFadeAlpha(float alpha) override;

        const math::Matrix& GetTransform() const override;
        void PushNewTransform(const math::Matrix& transform) override;
        void PopTransform() override;

        void PushNewProjection(const math::Matrix& projection) override;
        void PopProjection() override;

        void PushNewViewTransform(const math::Matrix& transform) override;
        void PopViewTransform() override;

        const math::Quad& GetViewport() const override;
        CullResult Cull(const math::Quad& world_bb) const override;

        float GetDeltaTime() const override;
        uint32_t GetTimestamp() const override;

    private:

        struct OffscreenPassData
        {
            math::Vector image_size;
            mono::ITexturePtr offscreen_texture;
            sg_pass pass_handle;
        };
        OffscreenPassData m_color_pass;
        OffscreenPassData m_light_pass;
        OffscreenPassData m_color_post_light_pass;

        void MakeOrUpdateOffscreenPass(OffscreenPassData& offscreen_pass, const math::Vector& drawable_size) const;
        void DrawLights();

        void PrepareDraw();
        void EndDraw();

        math::Vector m_window_size;
        math::Vector m_drawable_size;
        math::Quad m_viewport;

        mono::Color::RGBA m_clear_color;
        mono::Color::RGBA m_ambient_shade;
        float m_screen_fade_alpha;

        std::stack<math::Matrix> m_projection_stack;
        std::stack<math::Matrix> m_model_stack;
        std::stack<math::Matrix> m_view_stack;

        std::unique_ptr<IPipeline> m_color_points_pipeline;
        std::unique_ptr<IPipeline> m_color_lines_pipeline;
        std::unique_ptr<IPipeline> m_color_lines_indices_pipeline;
        std::unique_ptr<IPipeline> m_color_line_strip_pipeline;
        std::unique_ptr<IPipeline> m_color_line_strip_indices_pipeline;
        std::unique_ptr<IPipeline> m_color_triangles_pipeline;
        std::unique_ptr<IPipeline> m_particle_pipeline_one;
        std::unique_ptr<IPipeline> m_particle_pipeline_sa;
        std::unique_ptr<IPipeline> m_texture_pipeline;
        std::unique_ptr<IPipeline> m_texture_annotation_pipeline;
        std::unique_ptr<IPipeline> m_texture_pipeline_color;
        std::unique_ptr<IPipeline> m_sprite_pipeline;
        std::unique_ptr<IPipeline> m_fog_pipeline;
        std::unique_ptr<IPipeline> m_screen_pipeline;

        std::unique_ptr<ISampler> m_sampler;

        std::unique_ptr<IRenderBuffer> m_screen_vertices;
        std::unique_ptr<IRenderBuffer> m_screen_uv;
        std::unique_ptr<IElementBuffer> m_screen_indices;

        mono::ITexturePtr m_light_mask_texture;

        float m_delta_time_s = 0.0f;
        uint32_t m_timestamp = 0;

        std::vector<const IDrawable*> m_drawables[RenderPass::N_RENDER_PASS];

        struct LightData
        {
            math::Vector position;
            float radius;
            mono::Color::RGBA shade;
        };

        std::vector<LightData> m_lights;
        bool m_lighting_enabled;
    };
}

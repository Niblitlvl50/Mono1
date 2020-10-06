
#pragma once

#include "IRenderer.h"
#include "Color.h"
#include "Math/Vector.h"
#include "Math/Quad.h"
#include "Math/Matrix.h"
#include "ImGui.h"

#include <memory>
#include <stack>

namespace mono
{
    class IShader;
    class FrameBuffer;

    class Renderer : public IRenderer
    {
    public:

        Renderer();
        ~Renderer();

        void SetWindowSize(const math::Vector& window_size);
        void SetDrawableSize(const math::Vector& drawable_size);
        void SetViewport(const math::Quad& viewport);
        void SetDeltaAndTimestamp(uint32_t delta_ms, uint32_t timestamp);

        void DrawFrame();

        const math::Quad& GetViewport() const override;
        bool Cull(const math::Quad& world_bb) const override;

        uint32_t GetDeltaTimeMS() const override;
        uint32_t GetTimestamp() const override;

        void AddDrawable(const IDrawable* drawable) override;

        void DrawText(int font_id, const char* text, const math::Vector& pos, bool center, const mono::Color::RGBA& color) const override;
        void DrawSprite(const ISprite& sprite) const override;
        void DrawSprite(const ISprite& sprite, const math::Vector& offset) const override;
        void DrawSprite(
            const math::Quad& sprite_coords, const math::Vector& size, const math::Vector& offset, const ITexture* texture) const override;
        void DrawPoints(const std::vector<math::Vector>& points, const mono::Color::RGBA& color, float size) const override;
        void DrawLines(const std::vector<math::Vector>& linePoints, const mono::Color::RGBA& color, float width) const override;
        void DrawPolyline(const std::vector<math::Vector>& linePoints, const mono::Color::RGBA& color, float width) const override;
        void DrawClosedPolyline(const std::vector<math::Vector>& linePoints, const mono::Color::RGBA& color, float width) const override;
        void DrawQuad(const math::Quad& quad, const mono::Color::RGBA& color, float width) const override;
        void DrawFilledQuad(const math::Quad& quad, const mono::Color::RGBA& color) const override;
        void DrawCircle(const math::Vector& pos, float radie, int segments, float lineWidth, const mono::Color::RGBA& color) const override;

        void DrawGeometry(
            const std::vector<math::Vector>& vertices,
            const std::vector<math::Vector>& texture_coordinates,
            const std::vector<uint16_t>& indices,
            const ITexture* texture) override;

        void DrawGeometry(
            const IRenderBuffer* vertices,
            const IRenderBuffer* texture_coordinates,
            size_t offset,
            size_t count,
            const ITexture* texture) override;

        void DrawParticlePoints(
            const IRenderBuffer* position,
            const IRenderBuffer* rotation,
            const IRenderBuffer* color,
            const IRenderBuffer* point_size,
            const ITexture* texture,
            BlendMode blend_mode,
            size_t count) override;

        void DrawPolyline(
            const IRenderBuffer* vertices, const IRenderBuffer* colors, size_t offset, size_t count) override;

        void DrawTrianges(
            const mono::IRenderBuffer* vertices, const mono::IRenderBuffer* colors, const mono::IRenderBuffer* indices, size_t count) const override;

        void SetClearColor(const mono::Color::RGBA& color) override;

        void UseShader(IShader* shader) const override;
        void UseTexture(const ITexture* texture) const override;
        void ClearTexture() override;

        const math::Matrix& GetTransform() const override;
        void PushNewTransform(const math::Matrix& transform) override;
        void PopTransform() override;

        void PushNewProjection(const math::Matrix& projection) override;
        void PopProjection() override;

    private:

        void PrepareDraw();
        void EndDraw();

        math::Vector m_window_size;
        math::Vector m_drawable_size;
        math::Quad m_viewport;

        std::stack<math::Matrix> m_projection_stack;
        std::stack<math::Matrix> m_modelview_stack;

        uint32_t m_delta_time_ms = 0;
        uint32_t m_timestamp = 0;

        mutable uint32_t m_current_shader_id = -1;
        mutable uint32_t m_current_texture_id = -1;

        std::unique_ptr<IShader> m_color_shader;
        std::unique_ptr<IShader> m_texture_shader;
        std::unique_ptr<IShader> m_point_sprite_shader;
        std::unique_ptr<IShader> m_screen_shader;

        std::unique_ptr<FrameBuffer> m_frame_buffer;

        std::vector<const IDrawable*> m_drawables;
        ImGuiContext m_imgui_context;

        mono::Color::RGBA m_clear_color;
    };
}

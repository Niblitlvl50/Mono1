
#pragma once

#include "IRenderer.h"
#include "Math/Vector.h"
#include "Math/Quad.h"
#include "Math/Matrix.h"
#include "ImGui.h"

#include <memory>
#include <stack>

namespace mono
{
    class IShader;

    class Renderer : public IRenderer
    {
    public:

        Renderer();
        ~Renderer();

        void SetWindowSize(const math::Vector& window_size);
        void SetViewport(const math::Quad& viewport);
        virtual const math::Quad& GetViewport() const;
        virtual bool Cull(const math::Quad& world_bb) const;

        void SetDeltaAndTimestamp(uint32_t delta_ms, uint32_t timestamp);
        virtual uint32_t GetDeltaTimeMS() const;
        virtual uint32_t GetTimestamp() const;

        virtual void DrawFrame();
        virtual void AddDrawable(const IDrawable* drawable);

        virtual void DrawText(int font_id, const char* text, const math::Vector& pos, bool center, const mono::Color::RGBA& color) const;
        virtual void DrawSprite(const ISprite& sprite) const;
        virtual void DrawSprite(const ISprite& sprite, const math::Vector& offset) const;
        virtual void DrawSprite(
            const math::Quad& sprite_coords, const math::Vector& size, const math::Vector& offset, const ITexturePtr& texture) const;
        virtual void DrawPoints(const std::vector<math::Vector>& points, const mono::Color::RGBA& color, float size) const;
        virtual void DrawLines(const std::vector<math::Vector>& linePoints, const mono::Color::RGBA& color, float width) const;
        virtual void DrawPolyline(const std::vector<math::Vector>& linePoints, const mono::Color::RGBA& color, float width) const;
        virtual void DrawClosedPolyline(const std::vector<math::Vector>& linePoints, const mono::Color::RGBA& color, float width) const;
        virtual void DrawQuad(const math::Quad& quad, const mono::Color::RGBA& color, float width) const;
        virtual void DrawFilledQuad(const math::Quad& quad, const mono::Color::RGBA& color) const;
        virtual void DrawCircle(const math::Vector& pos, float radie, int segments, float lineWidth, const mono::Color::RGBA& color) const;

        virtual void DrawGeometry(const std::vector<math::Vector>& vertices,
                                  const std::vector<math::Vector>& texture_coordinates,
                                  const std::vector<uint16_t>& indices,
                                  const ITexturePtr& texture);

        virtual void DrawGeometry(const IRenderBuffer* vertices,
                                  const IRenderBuffer* texture_coordinates,
                                  size_t offset,
                                  size_t count,
                                  const ITexturePtr& texture);

        virtual void DrawParticlePoints(
            const IRenderBuffer* position,
            const IRenderBuffer* rotation,
            const IRenderBuffer* color,
            const IRenderBuffer* point_size,
            const ITexturePtr& texture,
            BlendMode blend_mode,
            size_t count);

        virtual void DrawPolyline(
            const IRenderBuffer* vertices, const IRenderBuffer* colors, size_t offset, size_t count);    

        void DrawTrianges(
            const mono::IRenderBuffer* vertices, const mono::IRenderBuffer* colors, const mono::IRenderBuffer* indices, size_t count) const;

        virtual void UseShader(IShader* shader) const;
        virtual void UseTexture(const ITexturePtr& texture) const;
        virtual void ClearTexture();

        virtual const math::Matrix& GetTransform() const;
        virtual void PushNewTransform(const math::Matrix& transform);
        virtual void PopTransform();

        virtual void PushNewProjection(const math::Matrix& projection);
        virtual void PopProjection();

    private:

        void PrepareDraw();
        void EndDraw();

        void doDrawTexts() const;

        math::Vector m_window_size;
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

        std::vector<const IDrawable*> m_drawables;
        ImGuiContext m_imgui_context;
    };
}

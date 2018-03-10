
#pragma once

#include "IRenderer.h"
#include "Math/Matrix.h"
#include "Shader/ShaderFwd.h"

namespace mono
{
    class Renderer : public IRenderer
    {
    public:

        Renderer(ICameraPtr camera);
        ~Renderer();

        virtual void DrawFrame();
        virtual void AddDrawable(const IDrawablePtr& drawable);

        virtual void DrawText(int font_id, const char* text, const math::Vector& pos, bool center, const mono::Color::RGBA& color) const;
        virtual void DrawSprite(const ISprite& sprite) const;
        virtual void DrawPoints(const std::vector<math::Vector>& points, const mono::Color::RGBA& color, float size) const;
        virtual void DrawLines(const std::vector<math::Vector>& linePoints, const mono::Color::RGBA& color, float width) const;
        virtual void DrawPolyline(const std::vector<math::Vector>& linePoints, const mono::Color::RGBA& color, float width) const;
        virtual void DrawClosedPolyline(const std::vector<math::Vector>& linePoints, const mono::Color::RGBA& color, float width) const;
        virtual void DrawQuad(const math::Quad& quad, const mono::Color::RGBA& color, float width) const;
        virtual void DrawFilledQuad(const math::Quad& quad, const mono::Color::RGBA& color) const;
        virtual void DrawCircle(const math::Vector& pos, float radie, int segments, float lineWidth, const mono::Color::RGBA& color) const;
        virtual void DrawShape(const std::vector<math::Vector>& shape1, const std::vector<math::Vector>& shape2, float morphGrade, const Color::RGBA& color);

        virtual void DrawGeometry(const std::vector<math::Vector>& vertices,
                                  const std::vector<math::Vector>& texture_coordinates,
                                  const std::vector<unsigned short>& indices,
                                  const ITexturePtr& texture);

        virtual void DrawGeometry(const IRenderBuffer* vertices,
                                  const IRenderBuffer* texture_coordinates,
                                  size_t offset,
                                  size_t count,
                                  const ITexturePtr& texture);

        virtual void DrawParticlePoints(const IRenderBuffer* position,
                                        const IRenderBuffer* color,
                                        const IRenderBuffer* point_size,
                                        const ITexturePtr& texture,
                                        size_t count);

        virtual void DrawPolyline(
            const IRenderBuffer* vertices, const IRenderBuffer* colors, size_t offset, size_t count);                             

        virtual void UseShader(IShader* shader) const;
        virtual void UseTexture(const ITexturePtr& texture) const;
        virtual void ClearTexture();

        virtual void PushGlobalTransform();

        virtual void PushNewTransform(const math::Matrix& transform);
        virtual const math::Matrix& GetCurrentTransform() const;

        virtual void PushNewProjection(const math::Matrix& projection);
        virtual const math::Matrix& GetCurrentProjection() const;

    private:

        void PrepareDraw();
        void EndDraw();

        void doDrawTexts() const;

        ICameraPtr mCamera;

        math::Matrix mProjectionMatrix;
        math::Matrix mModelView;

        math::Matrix m_current_projection;
        math::Matrix m_current_transform;

        mutable unsigned int m_currentShaderId = -1;
        mutable unsigned int m_currentTextureId = -1;

        std::unique_ptr<IColorShader> m_color_shader;
        std::unique_ptr<ITextureShader> m_texture_shader;
        std::unique_ptr<IMorphingShader> m_morph_shader;
        std::unique_ptr<IPointSpriteShader> m_point_sprite_shader;

        std::vector<IDrawablePtr> mDrawables;
    };
}

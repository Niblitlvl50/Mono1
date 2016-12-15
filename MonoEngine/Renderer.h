
#pragma once

#include "IRenderer.h"
#include "Math/Matrix.h"

namespace mono
{
    class IColorShader;
    class ITextureShader;
    class IMorphingShader;

    class Renderer : public IRenderer
    {
    public:

        Renderer(ICameraPtr camera, IWindowPtr window);

        virtual void DrawFrame();
        virtual void Update(unsigned int milliseconds);

        virtual void AddDrawable(const IDrawablePtr& drawable);
        virtual void AddUpdatable(const IUpdatablePtr& updatable);

        virtual void DrawText(int font_id, const char* text, const math::Vector& pos, bool center, const mono::Color::RGBA& color);
        virtual void DrawSprite(const ISprite& sprite) const;
        virtual void DrawPoints(const std::vector<math::Vector>& points, const mono::Color::RGBA& color, float size) const;
        virtual void DrawLines(const std::vector<math::Vector>& linePoints, const mono::Color::RGBA& color, float width) const;
        virtual void DrawPolyline(const std::vector<math::Vector>& linePoints, const mono::Color::RGBA& color, float width) const;
        virtual void DrawClosedPolyline(const std::vector<math::Vector>& linePoints, const mono::Color::RGBA& color, float width) const;
        virtual void DrawQuad(const math::Quad& quad, const mono::Color::RGBA& color, float width) const;
        virtual void DrawCircle(const math::Vector& pos, float radie, int segments, float lineWidth, const mono::Color::RGBA& color) const;
        virtual void DrawShape(const std::vector<math::Vector>& shape1, const std::vector<math::Vector>& shape2, float morphGrade, const Color::RGBA& color);

        virtual void DrawGeometry(const std::vector<math::Vector>& vertices,
                                  const std::vector<math::Vector>& texture_coordinates,
                                  const std::vector<unsigned short>& indices,
                                  const ITexturePtr& texture);

        virtual void UseShader(const IShaderPtr& shader) const;
        virtual void UseTexture(const ITexturePtr& texture) const;
        virtual void ClearTexture();

        virtual void PushNewTransform(const math::Matrix& transform);
        virtual const math::Matrix& GetCurrentTransform() const;

        virtual void PushNewProjection(const math::Matrix& projection);
        virtual const math::Matrix& GetCurrentProjection() const;

    private:

        void PrepareDraw();
        void EndDraw();

        void doDrawTexts() const;

        ICameraPtr mCamera;
        IWindowPtr mWindow;

        math::Matrix mProjectionMatrix;
        math::Matrix mModelView;

        math::Matrix m_current_projection;
        math::Matrix m_current_transform;

        mutable unsigned int m_currentShaderId;
        mutable unsigned int m_currentTextureId;

        std::shared_ptr<IColorShader> mColorShader;
        std::shared_ptr<ITextureShader> mTextureShader;
        std::shared_ptr<IMorphingShader> m_morphShader;

        std::vector<IDrawablePtr> mDrawables;
        std::vector<IUpdatablePtr> mUpdatables;
    };
}

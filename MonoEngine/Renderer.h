/*
 *  MOGLRenderer.h
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-08.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "IRenderer.h"
#include "TextDefinition.h"
#include "Matrix.h"

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
        
        virtual void DrawText(const std::string& text, const math::Vector2f& pos, bool center, const mono::Color::RGBA& color);
        virtual void DrawSprite(const ISprite& sprite) const;
        virtual void DrawPoints(const std::vector<math::Vector2f>& points, const mono::Color::RGBA& color, float size) const;
        virtual void DrawLines(const std::vector<math::Vector2f>& linePoints, const mono::Color::RGBA& color, float width) const;
        virtual void DrawPolyline(const std::vector<math::Vector2f>& linePoints, const mono::Color::RGBA& color, float width) const;
        virtual void DrawQuad(const math::Quad& quad, const mono::Color::RGBA& color, float width) const;
        virtual void DrawCircle(const math::Vector2f& pos, float radie, int segments, float lineWidth, const mono::Color::RGBA& color) const;
        virtual void DrawShape(const std::vector<math::Vector2f>& shape1, const std::vector<math::Vector2f>& shape2, float morphGrade, const Color::RGBA& color);

        virtual void UseShader(const IShaderPtr& shader) const;

        virtual void PushNewTransform(const math::Matrix& transform);
        virtual const math::Matrix& GetCurrentTransform() const;

    private:
        
        void PrepareDraw();
        void EndDraw();

        void doDrawTexts() const;

        ICameraPtr mCamera;
        IWindowPtr mWindow;

        math::Matrix mProjectionMatrix;
        math::Matrix mModelView;
        math::Matrix mCurrentTransform;

        std::shared_ptr<IColorShader> mColorShader;
        std::shared_ptr<ITextureShader> mTextureShader;
        std::shared_ptr<IMorphingShader> m_morphShader;

        std::vector<IDrawablePtr> mDrawables;
        std::vector<IUpdatablePtr> mUpdatables;
        std::vector<TextDefinition> mTexts;
    };
}


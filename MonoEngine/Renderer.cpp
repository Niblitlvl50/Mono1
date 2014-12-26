/*
 *  OGLRenderer.cpp
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-08.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Renderer.h"
#include "IEntity.h"
#include "ICamera.h"
#include "IWindow.h"
#include "Vector2f.h"
#include "Quad.h"
#include "Matrix.h"
#include "MathFunctions.h"
#include "RenderUtils.h"
#include "TextFunctions.h"

#include "IShaderFactory.h"
#include "ITextureShader.h"
#include "IColorShader.h"

using namespace mono;

Renderer::Renderer(ICameraPtr camera, IWindowPtr window)
    : mCamera(camera),
      mWindow(window)
{
    mColorShader = shaderFactory->CreateColorShader();
    mTextureShader = shaderFactory->CreateTextureShader();
}

void Renderer::PrepareDraw()
{
    const math::Quad& viewport = mCamera->GetViewport();
    mProjection = math::Ortho(0.0f, viewport.mB.mX, 0.0f, viewport.mB.mY, 0.0f, 10.0f);

    math::Identity(mModelView);
    math::Translate(mModelView, -viewport.mA);

    mWindow->MakeCurrent();
}

void Renderer::EndDraw()
{
    mWindow->SwapBuffers();

    // Clear all the stuff once the frame has been drawn
    mDrawables.clear();
    mUpdatables.clear();
    mTexts.clear();
}

void Renderer::DrawFrame()
{
    PrepareDraw();
    
    for(const auto& drawable : mDrawables)
    {
        mCurrentTransform = mModelView;

        const math::Quad& bounds = drawable->BoundingBox();
        const math::Quad& viewport = mCamera->GetViewport();
        const math::Quad camQuad(viewport.mA, viewport.mA + viewport.mB);

        // Make sure the entity is visible
        const bool visible = math::QuadOverlaps(camQuad, bounds);
        if(visible)
            drawable->doDraw(*this);
    }
    
    // Draw all the texts after all the entities.
    mCurrentTransform = mModelView;
    doDrawTexts();

    EndDraw();
}

void Renderer::Update(unsigned int milliseconds)
{
    mCamera->Update(milliseconds);
    
    for(auto& updatable : mUpdatables)
        updatable->doUpdate(milliseconds);
}

void Renderer::AddDrawable(IDrawablePtr drawable)
{
    mDrawables.push_back(drawable);
}

void Renderer::AddUpdatable(IUpdatablePtr updatable)
{
    mUpdatables.push_back(updatable);
}

void Renderer::DrawText(const std::string& text, const math::Vector2f& pos, bool center, const mono::Color& color)
{
    TextDefinition def = GenerateVertexDataFromString(text, pos, center);
    def.color = color;

    // Save the text in the collection
    mTexts.push_back(def);
}

void Renderer::DrawSprite(const Sprite& sprite) const
{
    mTextureShader->Use();
    mTextureShader->LoadMatrices(mProjection, mCurrentTransform);
    ::DrawSprite(sprite, mTextureShader);
}

void Renderer::doDrawTexts() const
{
    UseFont();
    
    mTextureShader->Use();
    mTextureShader->LoadMatrices(mProjection, mCurrentTransform);
    ::DrawTexts(mTexts, mTextureShader);
}

void Renderer::DrawPoints(const std::vector<math::Vector2f>& points, const mono::Color& color, float size) const
{
    mColorShader->Use();
    mColorShader->LoadMatrices(mProjection, mCurrentTransform);
    ::DrawPoints(points, color, size, mColorShader);
}

void Renderer::DrawLines(const std::vector<math::Vector2f>& linePoints, const mono::Color& color, float width) const
{
    mColorShader->Use();
    mColorShader->LoadMatrices(mProjection, mCurrentTransform);
    ::DrawLines(linePoints, color, width, mColorShader);
}

void Renderer::DrawQuad(const math::Quad& quad, const mono::Color& color, float width) const
{
    mColorShader->Use();
    mColorShader->LoadMatrices(mProjection, mCurrentTransform);
    ::DrawQuad(quad, color, width, mColorShader);
}

void Renderer::PushNewTransform(const math::Matrix& transform)
{
    mCurrentTransform = transform;
}

const math::Matrix& Renderer::GetCurrentTransform() const
{
    return mCurrentTransform;
}



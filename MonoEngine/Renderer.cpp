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
#include "ShaderFunctions.h"
#include "ITextureShader.h"
#include "IColorShader.h"
#include "IMorphingShader.h"

#include "ISprite.h"
#include "ITexture.h"


using namespace mono;

Renderer::Renderer(ICameraPtr camera, IWindowPtr window)
    : mCamera(camera),
      mWindow(window),
      m_currentTextureId(-1)
{
    mColorShader = GetShaderFactory()->CreateColorShader();
    mTextureShader = GetShaderFactory()->CreateTextureShader();
    m_morphShader = GetShaderFactory()->CreateMorphingShader();
}

void Renderer::PrepareDraw()
{
    const math::Quad& viewport = mCamera->GetViewport();
    mProjectionMatrix = math::Ortho(0.0f, viewport.mB.x, 0.0f, viewport.mB.y, -10.0f, 10.0f);

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

void Renderer::AddDrawable(const IDrawablePtr& drawable)
{
    mDrawables.push_back(drawable);
}

void Renderer::AddUpdatable(const IUpdatablePtr& updatable)
{
    mUpdatables.push_back(updatable);
}

void Renderer::DrawText(const char* text, const math::Vector2f& pos, bool center, const mono::Color::RGBA& color)
{
    TextDefinition def = mono::GenerateVertexDataFromString(text, pos, center);
    def.color = color;

    // Save the text in the collection
    mTexts.push_back(def);
}

void Renderer::doDrawTexts() const
{
    const mono::ITexturePtr& texture = mono::GetFontTexture();
    if(!texture)
        return;

    UseTexture(texture);

    UseShader(mTextureShader);
    ::DrawTexts(mTexts, mTextureShader);
}

void Renderer::DrawSprite(const ISprite& sprite) const
{
    UseShader(mTextureShader);
    UseTexture(sprite.GetTexture());
    ::DrawSprite(sprite, mTextureShader);
}

void Renderer::DrawPoints(const std::vector<math::Vector2f>& points, const mono::Color::RGBA& color, float size) const
{
    UseShader(mColorShader);
    ::DrawPoints(points, color, size, mColorShader);
}

void Renderer::DrawLines(const std::vector<math::Vector2f>& linePoints, const mono::Color::RGBA& color, float width) const
{
    UseShader(mColorShader);
    ::DrawLines(linePoints, color, width, mColorShader);
}

void Renderer::DrawPolyline(const std::vector<math::Vector2f>& linePoints, const mono::Color::RGBA& color, float width) const
{
    UseShader(mColorShader);
    ::DrawLine(linePoints, color, width, mColorShader);
}

void Renderer::DrawClosedPolyline(const std::vector<math::Vector2f>& linePoints, const mono::Color::RGBA& color, float width) const
{
    UseShader(mColorShader);
    ::DrawClosedLine(linePoints, color, width, mColorShader);
}

void Renderer::DrawQuad(const math::Quad& quad, const mono::Color::RGBA& color, float width) const
{
    UseShader(mColorShader);
    ::DrawQuad(quad, color, width, mColorShader);
}

void Renderer::DrawCircle(const math::Vector2f& pos, float radie, int segments, float lineWidth, const mono::Color::RGBA& color) const
{
    UseShader(mColorShader);
    ::DrawCircle(pos, radie, segments, lineWidth, color, mColorShader);
}

void Renderer::DrawShape(const std::vector<math::Vector2f>& shape1, const std::vector<math::Vector2f>& shape2, float morphGrade, const mono::Color::RGBA& color)
{
    UseShader(m_morphShader);

    m_morphShader->SetMorphGrade(morphGrade);
    ::DrawShape(shape1, shape2, color, m_morphShader);
}

void Renderer::UseShader(const IShaderPtr& shader) const
{
    shader->Use();
    shader->LoadModelViewMatrix(mCurrentTransform);
    shader->LoadProjectionMatrix(mProjectionMatrix);
}

void Renderer::UseTexture(const ITexturePtr& texture) const
{
    const unsigned int id = texture->Id();
    if(id == m_currentTextureId)
        return;

    texture->Use();
    m_currentTextureId = texture->Id();
}

void Renderer::ClearTexture()
{

    m_currentTextureId = -1;
}

void Renderer::PushNewTransform(const math::Matrix& transform)
{
    mCurrentTransform = transform;
}

const math::Matrix& Renderer::GetCurrentTransform() const
{
    return mCurrentTransform;
}



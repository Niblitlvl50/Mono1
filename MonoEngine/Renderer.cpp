/*
 *  OGLRenderer.cpp
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-08.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Renderer.h"
#include "SysOpenGL.h"
#include "IEntity.h"
#include "ICamera.h"
#include "IWindow.h"
#include "Vector2f.h"
#include "Quad.h"
#include "MathFunctions.h"
#include "RenderUtils.h"

using namespace mono;

Renderer::Renderer(ICameraPtr camera, IWindowPtr window)
    : mCamera(camera),
      mWindow(window),
      mDrawBB(false)
{ }

void Renderer::PrepareDraw() const
{
    const math::Quad& viewport = mCamera->GetViewport();
        
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    OGL::glOrtho(0.0f, viewport.mB.mX, 0.0f, viewport.mB.mY, 0.0f, 10.0f);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glTranslatef(-viewport.mA.mX, -viewport.mA.mY, 0.0f);
}

void Renderer::EndDraw() const
{
    mWindow->SwapBuffers();
}

void Renderer::DrawFrame()
{
    PrepareDraw();
    
    for(auto it = mDrawables.begin(), end = mDrawables.end(); it != end; ++it)
    {
        const OGL::OGLPushPopMatrix raii;

        const IDrawablePtr drawable = *it;
        const math::Quad& bounds = drawable->BoundingBox();
        
        if(mDrawBB)
            DrawQuad(bounds);
        
        const math::Quad& viewport = mCamera->GetViewport();
        const math::Quad camQuad(viewport.mA, viewport.mA + viewport.mB);
        const bool visible = math::QuadOverlaps(camQuad, bounds);
        if(visible)
            drawable->doDraw(*this);
    }
    
    // Draw all the texts after all the entities. 
    DrawTextFromDefinitions(mTexts);
    
    EndDraw();
}

void Renderer::Update(unsigned int milliseconds)
{
    mCamera->Update(milliseconds);
    
    // Should i do collision detection here or after the update?
    
    for(auto it = mUpdatables.begin(), end = mUpdatables.end(); it != end; ++it)
    {
        IUpdatablePtr updatable = *it;
        updatable->doUpdate(milliseconds);
    }
}

void Renderer::AddEntity(IEntityPtr entity)
{
    mDrawables.push_back(entity);
    mUpdatables.push_back(entity);
}

void Renderer::AddDrawable(IDrawablePtr drawable)
{
    mDrawables.push_back(drawable);
}

void Renderer::AddUpdatable(IUpdatablePtr updatable)
{
    mUpdatables.push_back(updatable);
}

void Renderer::DrawSprite(const Sprite& sprite) const
{
    ::DrawSprite(sprite);
}

void Renderer::DrawText(const std::string& text, const math::Vector2f& pos, bool center, Color color)
{
    TextDefinition def = GenerateVertexDataFromString(text, pos, center);
    def.color[0] = color[0];
    def.color[1] = color[1];
    def.color[2] = color[2];
    def.color[3] = color[3];
    
    // Save the text in the collection
    mTexts.push_back(def);
}



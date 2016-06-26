/*
 *  IRenderer.h
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-08.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "MonoPtrFwd.h"
#include "MathFwd.h"

#include <vector>

namespace mono
{
    struct IRenderer
    {
        virtual ~IRenderer()
        { }
		
        virtual void AddDrawable(const IDrawablePtr& drawable) = 0;
        virtual void AddUpdatable(const IUpdatablePtr& updatable) = 0;
        
        virtual void DrawText(const char* text, const math::Vector2f& pos, bool center, const Color::RGBA& color) = 0;
        virtual void DrawSprite(const ISprite& sprite) const = 0;
        virtual void DrawPoints(const std::vector<math::Vector2f>& points, const Color::RGBA& color, float size) const = 0;
        virtual void DrawLines(const std::vector<math::Vector2f>& linePoints, const Color::RGBA& color, float width) const = 0;
        virtual void DrawPolyline(const std::vector<math::Vector2f>& linePoints, const Color::RGBA& color, float width) const = 0;
        virtual void DrawClosedPolyline(const std::vector<math::Vector2f>& linePoints, const Color::RGBA& color, float width) const = 0;
        virtual void DrawQuad(const math::Quad& quad, const Color::RGBA& color, float width) const = 0;
        virtual void DrawCircle(const math::Vector2f& pos, float radie, int segments, float lineWidth, const Color::RGBA& color) const = 0;
        virtual void DrawShape(const std::vector<math::Vector2f>& shape1, const std::vector<math::Vector2f>& shape2, float morphGrade, const Color::RGBA& color) = 0;

        virtual void UseShader(const IShaderPtr& shader) const = 0;

        virtual void PushNewTransform(const math::Matrix& transform) = 0;
        virtual const math::Matrix& GetCurrentTransform() const = 0;
    };

}


//
//  RenderUtils.cpp
//  Mono1
//
//  Created by Niblit on 2013-03-17.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "RenderUtils.h"
#include "Quad.h"
#include "Sprite.h"
#include "SysOpenGL.h"
#include "MathFunctions.h"
#include "TextDefinition.h"
#include "ITexture.h"
#include "IColorShader.h"
#include "ITextureShader.h"

#include <cmath>

void mono::DrawQuad(const math::Quad& quad,
                    const mono::Color& color,
                    float width,
                    const std::shared_ptr<IColorShader>& shader)
{
    const std::vector<math::Vector2f> vertices = { math::Vector2f(quad.mA.mX, quad.mA.mY),
                                                   math::Vector2f(quad.mB.mX, quad.mA.mY),
                                                   math::Vector2f(quad.mB.mX, quad.mB.mY),
                                                   math::Vector2f(quad.mA.mX, quad.mB.mY),
                                                   math::Vector2f(quad.mA.mX, quad.mA.mY) };

    DrawLine(vertices, color, width, shader);
}

void mono::DrawCircle(const math::Vector2f& position,
                      float radie,
                      unsigned int segments,
                      const mono::Color& color,
                      const std::shared_ptr<IColorShader>& shader)
{
    std::vector<math::Vector2f> vertices;
    vertices.reserve(segments);

    const float coef = 2.0f * math::PI() / float(segments);
    
	for(unsigned int index = 0; index < segments; ++index)
	{
		const float radians = index * coef;
		const float x = radie * std::cos(radians) + position.mX;
		const float y = radie * std::sin(radians) + position.mY;

        vertices.emplace_back(x, y);
	}

    DrawLine(vertices, color, 1.0f, shader);
}

void mono::DrawSprite(const mono::Sprite& sprite, const std::shared_ptr<ITextureShader>& shader)
{
    static const float vertices[] = { -0.5f, -0.5f,
                                      -0.5f,  0.5f,
                                       0.5f,  0.5f,
                                       0.5f, -0.5f };
    static const unsigned short indices[] = { 0, 2, 1, 0, 3, 2 };

    const math::Quad& quad = sprite.GetTextureCoords();
    const float coords[] = { quad.mA.mX, quad.mA.mY,
                             quad.mA.mX, quad.mB.mY,
                             quad.mB.mX, quad.mB.mY,
                             quad.mB.mX, quad.mA.mY };
    
    shader->SetShade(sprite.GetShade());
    shader->SetAlphaTexture(false);
    
    sprite.GetTexture()->Use();

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(shader->GetPositionAttributeLocation(), 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(shader->GetTextureAttributeLocation(), 2, GL_FLOAT, GL_FALSE, 0, coords);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void mono::DrawTexts(const std::vector<TextDefinition>& texts, const std::shared_ptr<ITextureShader>& shader)
{
    if(texts.empty())
        return;

    shader->SetAlphaTexture(true);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    for(const mono::TextDefinition& text : texts)
    {
        shader->SetShade(text.color);

        glVertexAttribPointer(shader->GetPositionAttributeLocation(), 2, GL_FLOAT, GL_FALSE, 0, text.vertices.data());
        glVertexAttribPointer(shader->GetTextureAttributeLocation(), 2, GL_FLOAT, GL_FALSE, 0, text.texcoords.data());

        // Number of chars in the text, times 3 since each triangle contains 3 vertices,
        // times 2 since each char containts two triangles.
        const int verticesToDraw = (int)text.chars * 3 * 2;

        glDrawArrays(GL_TRIANGLES, 0, verticesToDraw);
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void mono::DrawLine(const std::vector<math::Vector2f>& vertices,
                    const mono::Color& color,
                    float width,
                    const std::shared_ptr<IColorShader>& shader)
{
    std::vector<mono::Color> colors(vertices.size());
    std::fill(colors.begin(), colors.end(), color);

    glLineWidth(width);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(shader->GetPositionAttributeLocation(), 2, GL_FLOAT, GL_FALSE, 0, vertices.data());
    glVertexAttribPointer(shader->GetColorAttributeLocation(), 4, GL_FLOAT, GL_FALSE, 0, colors.data());

    glDrawArrays(GL_LINE_STRIP, 0, static_cast<int>(vertices.size()));

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}


void mono::DrawLines(const std::vector<math::Vector2f>& vertices,
                     const mono::Color& color,
                     float width,
                     const std::shared_ptr<IColorShader>& shader)
{
    std::vector<mono::Color> colors(vertices.size());
    std::fill(colors.begin(), colors.end(), color);

    glLineWidth(width);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(shader->GetPositionAttributeLocation(), 2, GL_FLOAT, GL_FALSE, 0, vertices.data());
    glVertexAttribPointer(shader->GetColorAttributeLocation(), 4, GL_FLOAT, GL_FALSE, 0, colors.data());

    glDrawArrays(GL_LINES, 0, static_cast<int>(vertices.size()));

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void mono::DrawPoints(const std::vector<math::Vector2f>& vertices,
                      const mono::Color& color,
                      float size,
                      const std::shared_ptr<IColorShader>& shader)
{
    std::vector<mono::Color> colors(vertices.size());
    std::fill(colors.begin(), colors.end(), color);

    glPointSize(size);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(shader->GetPositionAttributeLocation(), 2, GL_FLOAT, GL_FALSE, 0, vertices.data());
    glVertexAttribPointer(shader->GetColorAttributeLocation(), 4, GL_FLOAT, GL_FALSE, 0, colors.data());

    glDrawArrays(GL_POINTS, 0, static_cast<int>(vertices.size()));

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}



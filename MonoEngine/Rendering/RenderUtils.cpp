
#include "RenderUtils.h"
#include "IRenderBuffer.h"
#include "Shader/IColorShader.h"
#include "Shader/ITextureShader.h"
#include "Shader/IPointSpriteShader.h"
#include "Sprite/ISprite.h"
#include "Text/TextDefinition.h"
#include "Texture/ITexture.h"

#include "Math/MathFunctions.h"
#include "Math/Quad.h"

#include "System/OpenGL.h"

#include <cmath>
#include <cassert>
#include <cstdio>

void mono::DrawQuad(const math::Quad& quad,
                    const mono::Color::RGBA& color,
                    float width,
                    IColorShader* shader)
{
    const std::vector<math::Vector> vertices = {
        math::Vector(quad.mA.x, quad.mA.y),
        math::Vector(quad.mB.x, quad.mA.y),
        math::Vector(quad.mB.x, quad.mB.y),
        math::Vector(quad.mA.x, quad.mB.y)
    };

    DrawClosedLine(vertices, color, width, shader);
}

void mono::DrawFilledQuad(const math::Quad& quad, const mono::Color::RGBA& color, IColorShader* shader)
{
    const float vertices[] = {
        quad.mA.x, quad.mA.y,
        quad.mB.x, quad.mA.y,
        quad.mB.x, quad.mB.y,
        quad.mA.x, quad.mB.y
    };

    const float colors[] = {
        color.red, color.green, color.blue, color.alpha,
        color.red, color.green, color.blue, color.alpha,
        color.red, color.green, color.blue, color.alpha,
        color.red, color.green, color.blue, color.alpha
    };

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(shader->GetPositionAttributeLocation(), 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(shader->GetColorAttributeLocation(), 4, GL_FLOAT, GL_FALSE, 0, colors);

    glDrawArrays(GL_QUADS, 0, 4);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void mono::DrawCircle(const math::Vector& position,
                      float radie,
                      unsigned int segments,
                      float lineWidth,
                      const mono::Color::RGBA& color,
                      IColorShader* shader)
{
    std::vector<math::Vector> vertices;
    vertices.reserve(segments);

    const float coef = 2.0f * math::PI() / float(segments);
    
	for(unsigned int index = 0; index < segments; ++index)
	{
		const float radians = index * coef;
		const float x = radie * std::cos(radians) + position.x;
		const float y = radie * std::sin(radians) + position.y;

        vertices.emplace_back(x, y);
	}

    DrawClosedLine(vertices, color, lineWidth, shader);
}

void mono::DrawSprite(
    const math::Quad& sprite_coords, const math::Vector& size, const math::Vector& offset, float pixels_per_meter, ITextureShader* shader)
{
    const math::Vector& sprite_size = size / pixels_per_meter / 2.0f;

    const math::Vector vertices[] = {
        math::Vector(-sprite_size.x, -sprite_size.y) + offset,
        math::Vector(-sprite_size.x,  sprite_size.y) + offset,
        math::Vector( sprite_size.x,  sprite_size.y) + offset,
        math::Vector( sprite_size.x, -sprite_size.y) + offset
    };

    constexpr unsigned short indices[] = {
        0, 1, 2, 0, 2, 3
    };

    const float texture_coords[] = {
        sprite_coords.mA.x, sprite_coords.mA.y,
        sprite_coords.mA.x, sprite_coords.mB.y,
        sprite_coords.mB.x, sprite_coords.mB.y,
        sprite_coords.mB.x, sprite_coords.mA.y
    };
    
    //shader->SetShade(sprite.GetShade());
    shader->SetShade(mono::Color::RGBA(1, 1, 1));
    shader->SetAlphaTexture(false);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(shader->GetPositionAttributeLocation(), 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(shader->GetTextureAttributeLocation(), 2, GL_FLOAT, GL_FALSE, 0, texture_coords);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void mono::DrawText(const TextDefinition& text, ITextureShader* shader)
{
    shader->SetAlphaTexture(true);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    shader->SetShade(text.color);

    glVertexAttribPointer(shader->GetPositionAttributeLocation(), 2, GL_FLOAT, GL_FALSE, 0, text.vertices.data());
    glVertexAttribPointer(shader->GetTextureAttributeLocation(), 2, GL_FLOAT, GL_FALSE, 0, text.texcoords.data());

    // Number of chars in the text, times 3 since each triangle contains 3 vertices,
    // times 2 since each char containts two triangles.
    const int verticesToDraw = (int)text.chars * 3 * 2;

    glDrawArrays(GL_TRIANGLES, 0, verticesToDraw);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void mono::DrawLine(
    const std::vector<math::Vector>& vertices, const mono::Color::RGBA& color, float width, IColorShader* shader)
{
    std::vector<mono::Color::RGBA> colors(vertices.size());
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

void mono::DrawClosedLine(
    const std::vector<math::Vector>& vertices, const mono::Color::RGBA& color, float width, IColorShader* shader)
{
    std::vector<mono::Color::RGBA> colors(vertices.size());
    std::fill(colors.begin(), colors.end(), color);

    glLineWidth(width);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(shader->GetPositionAttributeLocation(), 2, GL_FLOAT, GL_FALSE, 0, vertices.data());
    glVertexAttribPointer(shader->GetColorAttributeLocation(), 4, GL_FLOAT, GL_FALSE, 0, colors.data());

    glDrawArrays(GL_LINE_LOOP, 0, static_cast<int>(vertices.size()));

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void mono::DrawLines(
    const std::vector<math::Vector>& vertices, const mono::Color::RGBA& color, float width, IColorShader* shader)
{
    std::vector<mono::Color::RGBA> colors(vertices.size());
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

void mono::DrawPoints(
    const std::vector<math::Vector>& vertices, const mono::Color::RGBA& color, float size, IColorShader* shader)
{
    std::vector<mono::Color::RGBA> colors(vertices.size());
    std::fill(colors.begin(), colors.end(), color);

    shader->SetPointSize(size);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(shader->GetPositionAttributeLocation(), 2, GL_FLOAT, GL_FALSE, 0, vertices.data());
    glVertexAttribPointer(shader->GetColorAttributeLocation(), 4, GL_FLOAT, GL_FALSE, 0, colors.data());

    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(vertices.size()));

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void mono::DrawTexturedGeometry(const std::vector<math::Vector>& vertices,
                                const std::vector<math::Vector>& texture_coordinates,
                                const std::vector<unsigned short>& indices,
                                ITextureShader* shader)
{
    shader->SetShade(mono::Color::RGBA());
    shader->SetAlphaTexture(false);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(shader->GetPositionAttributeLocation(), 2, GL_FLOAT, GL_FALSE, 0, vertices.data());
    glVertexAttribPointer(shader->GetTextureAttributeLocation(), 2, GL_FLOAT, GL_FALSE, 0, texture_coordinates.data());

    glDrawElements(GL_TRIANGLE_FAN, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_SHORT, indices.data());

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void mono::DrawTexturedGeometry(const mono::IRenderBuffer* vertices,
                                const mono::IRenderBuffer* texture_coordinates,
                                size_t offset,
                                size_t count,
                                ITextureShader* shader)
{
    shader->SetShade(mono::Color::RGBA());
    shader->SetAlphaTexture(false);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    vertices->Use();
    glVertexAttribPointer(shader->GetPositionAttributeLocation(), 2, GL_FLOAT, GL_FALSE, 0, 0);

    texture_coordinates->Use();
    glVertexAttribPointer(shader->GetTextureAttributeLocation(), 2, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLE_FAN, offset, count);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    ClearRenderBuffer();
}

void mono::DrawTrianges(
    const mono::IRenderBuffer* vertices,
    const mono::IRenderBuffer* colors,
    const mono::IRenderBuffer* indices,
    size_t count,
    IColorShader* shader)
{
    shader->SetPointSize(5.0f);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
 
    vertices->Use();
    glVertexAttribPointer(shader->GetPositionAttributeLocation(), 2, GL_FLOAT, GL_FALSE, 0, 0);
 
    colors->Use();
    glVertexAttribPointer(shader->GetColorAttributeLocation(), 4, GL_FLOAT, GL_FALSE, 0, 0);
 
    glDrawArrays(GL_TRIANGLE_STRIP, 0, count);
    //glDrawArrays(GL_POINTS, 0, count);
 
    //indices->Use();
    //glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    ClearRenderBuffer();
}

void mono::DrawParticlePoints(
    const IRenderBuffer* position,
    const IRenderBuffer* color,
    const IRenderBuffer* point_size,
    size_t count,
    BlendMode blend_mode,
    IPointSpriteShader* shader)
{
    glEnable(GL_POINT_SPRITE);
    glDepthMask(GL_FALSE);

    if(blend_mode == BlendMode::ONE)
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    position->Use();
    glVertexAttribPointer(shader->GetPositionAttribute(), 2, GL_FLOAT, GL_FALSE, 0, 0);

    color->Use();
    glVertexAttribPointer(shader->GetColorAttribute(), 4, GL_FLOAT, GL_FALSE, 0, 0);

    point_size->Use();
    glVertexAttribPointer(shader->GetPointSizeAttribute(), 1, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_POINTS, 0, count);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    glDisable(GL_POINT_SPRITE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_TRUE);

    ClearRenderBuffer();
}

void mono::DrawPolyline(
    const mono::IRenderBuffer* vertices,
    const mono::IRenderBuffer* colors,
    size_t offset,
    size_t count,
    float width,
    IColorShader* shader)
{
    glLineWidth(width);
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    vertices->Use();
    glVertexAttribPointer(shader->GetPositionAttributeLocation(), 2, GL_FLOAT, GL_FALSE, 0, 0);

    colors->Use();
    glVertexAttribPointer(shader->GetColorAttributeLocation(), 2, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_LINE_LOOP, offset, count);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    ClearRenderBuffer();
}

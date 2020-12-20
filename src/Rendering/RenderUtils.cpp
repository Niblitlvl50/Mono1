
#include "RenderUtils.h"
#include "RenderBuffer/IRenderBuffer.h"
#include "Shader/ColorShader.h"
#include "Shader/TextureShader.h"
#include "Shader/PointSpriteShader.h"
#include "Shader/ScreenShader.h"
#include "Shader/SpriteShader.h"
#include "Sprite/ISprite.h"
#include "Text/TextDefinition.h"
#include "Texture/ITexture.h"
#include "ErrorChecking.h"

#include "Math/MathFunctions.h"
#include "Math/Quad.h"

#include "System/open_gl.h"

#include <cmath>
#include <cassert>
#include <cstdio>


void mono::DrawQuad(const math::Quad& quad,
                    const mono::Color::RGBA& color,
                    float width,
                    IShader* shader)
{
    const std::vector<math::Vector> vertices = {
        math::Vector(quad.mA.x, quad.mA.y),
        math::Vector(quad.mB.x, quad.mA.y),
        math::Vector(quad.mB.x, quad.mB.y),
        math::Vector(quad.mA.x, quad.mB.y)
    };

    DrawClosedLine(vertices, color, width, shader);
}

void mono::DrawFilledQuad(const math::Quad& quad, const mono::Color::RGBA& color, IShader* shader)
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

    glVertexAttribPointer(ColorShader::GetPositionAttribute(shader), 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(ColorShader::GetColorAttribute(shader), 4, GL_FLOAT, GL_FALSE, 0, colors);

    glDrawArrays(GL_QUADS, 0, 4);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void mono::DrawCircle(const math::Vector& position,
                      float radie,
                      uint32_t segments,
                      float line_width,
                      const mono::Color::RGBA& color,
                      IShader* shader)
{
    std::vector<math::Vector> vertices;
    vertices.reserve(segments);

    const float coef = 2.0f * math::PI() / float(segments);
    
    for(uint32_t index = 0; index < segments; ++index)
    {
        const float radians = index * coef;
        const float x = radie * std::cos(radians) + position.x;
        const float y = radie * std::sin(radians) + position.y;

        vertices.emplace_back(x, y);
    }

    DrawClosedLine(vertices, color, line_width, shader);
}

void mono::DrawFilledCircle(
    const math::Vector& position,
    const math::Vector& radie_size,
    uint32_t segments,
    const mono::Color::RGBA& color,
    IShader* shader)
{
    std::vector<math::Vector> vertices;
    vertices.reserve(segments + 1);
    vertices.push_back(position);

    const float coef = 2.0f * math::PI() / float(segments);
    
    for(uint32_t index = 0; index < segments; ++index)
    {
        const float radians = index * coef;
        const float x = radie_size.x * std::cos(radians) + position.x;
        const float y = radie_size.y * std::sin(radians) + position.y;

        vertices.emplace_back(x, y);
    }

    vertices.push_back(vertices[1]);

    std::vector<mono::Color::RGBA> colors(vertices.size());
    std::fill(colors.begin(), colors.end(), color);

    const uint32_t position_attribute = ColorShader::GetPositionAttribute(shader);
    const uint32_t color_attribute = ColorShader::GetColorAttribute(shader);

    glEnableVertexAttribArray(position_attribute);
    glEnableVertexAttribArray(color_attribute);

    glVertexAttribPointer(position_attribute, 2, GL_FLOAT, GL_FALSE, 0, vertices.data());
    glVertexAttribPointer(color_attribute, 4, GL_FLOAT, GL_FALSE, 0, colors.data());

    glDrawArrays(GL_TRIANGLE_FAN, 0, static_cast<int>(vertices.size()));

    glDisableVertexAttribArray(position_attribute);
    glDisableVertexAttribArray(color_attribute);
}

void mono::DrawSprite(
    const math::Vector& uv_upper_left, const math::Vector& uv_lower_right, const math::Vector& size, const math::Vector& offset, IShader* shader)
{
   const math::Vector& sprite_half_size = size / 2.0f;

    const math::Vector vertices[] = {
        math::Vector(-sprite_half_size.x, -sprite_half_size.y) + offset,
        math::Vector(-sprite_half_size.x,  sprite_half_size.y) + offset,
        math::Vector( sprite_half_size.x,  sprite_half_size.y) + offset,
        math::Vector( sprite_half_size.x, -sprite_half_size.y) + offset
    };

    const float texture_coords[] = {
        uv_upper_left.x, uv_upper_left.y,
        uv_upper_left.x, uv_lower_right.y,
        uv_lower_right.x, uv_lower_right.y,
        uv_lower_right.x, uv_upper_left.y
    };

    const float height_values[] = {
        0.0f, size.y, size.y, 0.0f
    };

    constexpr uint16_t indices[] = {
        0, 1, 2, 0, 2, 3
    };

    const uint32_t vertex_location = SpriteShader::GetPositionAttribute(shader);
    const uint32_t texture_location = SpriteShader::GetTextureAttribute(shader);
    const uint32_t height_location = SpriteShader::GetHeightAttribute(shader);

    glEnableVertexAttribArray(vertex_location);
    glEnableVertexAttribArray(texture_location);
    glEnableVertexAttribArray(height_location);

    glVertexAttribPointer(vertex_location, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(texture_location, 2, GL_FLOAT, GL_FALSE, 0, texture_coords);
    glVertexAttribPointer(height_location, 1, GL_FLOAT, GL_FALSE, 0, height_values);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);

    glDisableVertexAttribArray(vertex_location);
    glDisableVertexAttribArray(texture_location);
    glDisableVertexAttribArray(height_location);
}

void mono::DrawScreen(const math::Quad& texture_coordinates, const math::Vector& size, IShader* shader)
{
    const math::Vector vertices[] = {
        math::Vector(-size.x, -size.y),
        math::Vector(-size.x,  size.y),
        math::Vector( size.x,  size.y),
        math::Vector( size.x, -size.y),
    };

    constexpr uint16_t indices[] = {
        0, 1, 2, 0, 2, 3
    };

    const float texture_coords[] = {
        texture_coordinates.mA.x, texture_coordinates.mA.y,
        texture_coordinates.mA.x, texture_coordinates.mB.y,
        texture_coordinates.mB.x, texture_coordinates.mB.y,
        texture_coordinates.mB.x, texture_coordinates.mA.y
    };

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(ScreenShader::GetPositionAttribute(shader), 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(ScreenShader::GetTextureAttribute(shader), 2, GL_FLOAT, GL_FALSE, 0, texture_coords);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void mono::DrawText(const TextDefinition& text, IShader* shader)
{
    TextureShader::SetAlphaTexture(shader, true);
    TextureShader::SetShade(shader, text.color);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(TextureShader::GetPositionAttribute(shader), 2, GL_FLOAT, GL_FALSE, 0, text.vertices.data());
    glVertexAttribPointer(TextureShader::GetTextureAttribute(shader), 2, GL_FLOAT, GL_FALSE, 0, text.texcoords.data());

    // Number of chars in the text, times 3 since each triangle contains 3 vertices,
    // times 2 since each char containts two triangles.
    const int vertices_to_draw = (int)text.chars * 3 * 2;

    glDrawArrays(GL_TRIANGLES, 0, vertices_to_draw);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void mono::DrawLine(
    const std::vector<math::Vector>& vertices, const mono::Color::RGBA& color, float width, IShader* shader)
{
    std::vector<mono::Color::RGBA> colors(vertices.size());
    std::fill(colors.begin(), colors.end(), color);

    glLineWidth(width);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(ColorShader::GetPositionAttribute(shader), 2, GL_FLOAT, GL_FALSE, 0, vertices.data());
    glVertexAttribPointer(ColorShader::GetColorAttribute(shader), 4, GL_FLOAT, GL_FALSE, 0, colors.data());

    glDrawArrays(GL_LINE_STRIP, 0, static_cast<int>(vertices.size()));

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void mono::DrawClosedLine(
    const std::vector<math::Vector>& vertices, const mono::Color::RGBA& color, float width, IShader* shader)
{
    std::vector<mono::Color::RGBA> colors(vertices.size());
    std::fill(colors.begin(), colors.end(), color);

    glLineWidth(width);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(ColorShader::GetPositionAttribute(shader), 2, GL_FLOAT, GL_FALSE, 0, vertices.data());
    glVertexAttribPointer(ColorShader::GetColorAttribute(shader), 4, GL_FLOAT, GL_FALSE, 0, colors.data());

    glDrawArrays(GL_LINE_LOOP, 0, static_cast<int>(vertices.size()));

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void mono::DrawLines(
    const std::vector<math::Vector>& vertices, const mono::Color::RGBA& color, float width, IShader* shader)
{
    std::vector<mono::Color::RGBA> colors(vertices.size());
    std::fill(colors.begin(), colors.end(), color);

    glLineWidth(width);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(ColorShader::GetPositionAttribute(shader), 2, GL_FLOAT, GL_FALSE, 0, vertices.data());
    glVertexAttribPointer(ColorShader::GetColorAttribute(shader), 4, GL_FLOAT, GL_FALSE, 0, colors.data());

    glDrawArrays(GL_LINES, 0, static_cast<int>(vertices.size()));

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void mono::DrawPoints(
    const std::vector<math::Vector>& vertices, const mono::Color::RGBA& color, float size, IShader* shader)
{
    std::vector<mono::Color::RGBA> colors(vertices.size());
    std::fill(colors.begin(), colors.end(), color);

    ColorShader::SetPointSize(shader, size);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(ColorShader::GetPositionAttribute(shader), 2, GL_FLOAT, GL_FALSE, 0, vertices.data());
    glVertexAttribPointer(ColorShader::GetColorAttribute(shader), 4, GL_FLOAT, GL_FALSE, 0, colors.data());

    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(vertices.size()));

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void mono::DrawTexturedGeometry(const std::vector<math::Vector>& vertices,
                                const std::vector<math::Vector>& texture_coordinates,
                                const std::vector<uint16_t>& indices,
                                IShader* shader)
{
    TextureShader::SetShade(shader, mono::Color::RGBA());
    TextureShader::SetAlphaTexture(shader, false);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(TextureShader::GetPositionAttribute(shader), 2, GL_FLOAT, GL_FALSE, 0, vertices.data());
    glVertexAttribPointer(TextureShader::GetTextureAttribute(shader), 2, GL_FLOAT, GL_FALSE, 0, texture_coordinates.data());

    glDrawElements(GL_TRIANGLE_FAN, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_SHORT, indices.data());

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void mono::DrawTexturedGeometry(const mono::IRenderBuffer* vertices,
                                const mono::IRenderBuffer* texture_coordinates,
                                size_t offset,
                                size_t count,
                                IShader* shader)
{
    TextureShader::SetShade(shader, mono::Color::RGBA());
    TextureShader::SetAlphaTexture(shader, false);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    vertices->Use();
    glVertexAttribPointer(TextureShader::GetPositionAttribute(shader), 2, GL_FLOAT, GL_FALSE, 0, 0);

    texture_coordinates->Use();
    glVertexAttribPointer(TextureShader::GetTextureAttribute(shader), 2, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLE_FAN, offset, count);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    ClearRenderBuffer();
}

void mono::DrawTrianges(
    const mono::IRenderBuffer* vertices,
    const mono::IRenderBuffer* colors,
    const mono::IElementBuffer* indices,
    size_t count,
    IShader* shader)
{
    ColorShader::SetPointSize(shader, 5.0f);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
 
    vertices->Use();
    glVertexAttribPointer(ColorShader::GetPositionAttribute(shader), 2, GL_FLOAT, GL_FALSE, 0, 0);
 
    colors->Use();
    glVertexAttribPointer(ColorShader::GetColorAttribute(shader), 4, GL_FLOAT, GL_FALSE, 0, 0);

    indices->Use();
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    ClearRenderBuffer();
}

void mono::DrawParticlePoints(
    const IRenderBuffer* position,
    const IRenderBuffer* rotation,
    const IRenderBuffer* color,
    const IRenderBuffer* point_size,
    size_t count,
    BlendMode blend_mode,
    IShader* shader)
{
    glEnable(GL_POINT_SPRITE);
    glDepthMask(GL_FALSE);

    if(blend_mode == BlendMode::ONE)
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    
    position->Use();
    glVertexAttribPointer(PointSpriteShader::GetPositionAttribute(shader), 2, GL_FLOAT, GL_FALSE, 0, 0);

    rotation->Use();
    glVertexAttribPointer(PointSpriteShader::GetRotationAttribute(shader), 1, GL_FLOAT, GL_FALSE, 0, 0);

    color->Use();
    glVertexAttribPointer(PointSpriteShader::GetColorAttribute(shader), 4, GL_FLOAT, GL_FALSE, 0, 0);

    point_size->Use();
    glVertexAttribPointer(PointSpriteShader::GetPointSizeAttribute(shader), 1, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_POINTS, 0, count);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);

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
    IShader* shader)
{
    glLineWidth(width);
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    vertices->Use();
    glVertexAttribPointer(ColorShader::GetPositionAttribute(shader), 2, GL_FLOAT, GL_FALSE, 0, 0);

    colors->Use();
    glVertexAttribPointer(ColorShader::GetColorAttribute(shader), 2, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_LINE_LOOP, offset, count);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    ClearRenderBuffer();
}

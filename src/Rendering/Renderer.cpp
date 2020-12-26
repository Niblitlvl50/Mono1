
#include "Renderer.h"
#include "IDrawable.h"
#include "RenderUtils.h"
#include "Text/TextFunctions.h"
#include "Texture/ITexture.h"
#include "ErrorChecking.h"

#include "Shader/IShader.h"
#include "Shader/IShaderFactory.h"
#include "Shader/ShaderFunctions.h"
#include "Shader/TextureShader.h"
#include "Shader/SpriteShader.h"

#include "Sprite/ISprite.h"
#include "Sprite/SpriteProperties.h"

#include "Math/Vector.h"
#include "Math/Quad.h"
#include "Math/MathFunctions.h"

#include "ImGui.h"
#include "RenderBuffer/FrameBufferCD.h"

#include "System/open_gl.h"

using namespace mono;

namespace
{
    void SetupOpenGL()
    {
        glFrontFace(GL_CCW);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);

#ifndef __IPHONEOS__
        // To be able to use gl_PointSize in a vertex shader on regular
        // GLSL, we need to enable this thing, and its not avalible on
        // GLSL ES, that's why ifndef.
        glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
#endif
        glEnable(GL_POINT_SMOOTH);
        glEnable(GL_LINE_SMOOTH);
    }
}

Renderer::Renderer()
    : m_clear_color(0.7f, 0.7f, 0.7f)
{
    SetupOpenGL();

    m_color_shader = GetShaderFactory()->CreateColorShader();
    m_texture_shader = GetShaderFactory()->CreateTextureShader();
    m_point_sprite_shader = GetShaderFactory()->CreatePointSpriteShader();
    m_screen_shader = GetShaderFactory()->CreateScreenShader();
    m_sprite_shader = GetShaderFactory()->CreateSpriteShader();

    m_imgui_context.shader = GetShaderFactory()->CreateImGuiShader();
    mono::InitializeImGui(m_imgui_context);
}

Renderer::~Renderer()
{
    mono::DestroyImGui();
}

void Renderer::SetWindowSize(const math::Vector& window_size)
{
    m_window_size = window_size;
}

void Renderer::SetDrawableSize(const math::Vector& drawable_size)
{
    m_drawable_size = drawable_size;
}

void Renderer::SetViewport(const math::Quad& viewport)
{
    m_viewport = viewport;
}

const math::Quad& Renderer::GetViewport() const
{
    return m_viewport;
}

bool Renderer::Cull(const math::Quad& world_bb) const
{
    return math::QuadOverlaps(m_viewport, world_bb);
}

void Renderer::SetDeltaAndTimestamp(uint32_t delta_ms, uint32_t timestamp)
{
    m_delta_time_ms = delta_ms;
    m_timestamp = timestamp;
}

uint32_t Renderer::GetDeltaTimeMS() const
{
    return m_delta_time_ms;
}

uint32_t Renderer::GetTimestamp() const
{
    return m_timestamp;
}

void Renderer::PrepareDraw()
{
    m_projection_stack = { };
    m_view_stack = { };
    m_model_stack = { };

    const float viewport_width = math::Width(m_viewport);
    const float ratio = m_window_size.x / m_window_size.y;
    m_projection_stack.push(math::Ortho(0.0f, viewport_width, 0.0f, viewport_width / ratio, -10.0f, 10.0f));

    math::Matrix view_transform;
    math::Translate(view_transform, -m_viewport.mA);
    m_view_stack.push(view_transform);

    m_model_stack.push(math::Matrix()); // Push identity

    if(!m_frame_buffer || m_frame_buffer->Size() != m_drawable_size)
    {
        m_frame_buffer = std::make_unique<FrameBuffer>(m_drawable_size.x, m_drawable_size.y);
    }

    m_frame_buffer->Use();

    glClearColor(m_clear_color.red, m_clear_color.green, m_clear_color.blue, m_clear_color.alpha);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, m_drawable_size.x, m_drawable_size.y);
}

void Renderer::EndDraw()
{
    // Clear all the stuff once the frame has been drawn
    m_drawables.clear();
    //PROCESS_GL_ERRORS();

    m_imgui_context.window_size = m_window_size;
    m_imgui_context.drawable_size = m_drawable_size;
    mono::DrawImGui(m_imgui_context, *this);

    mono::ClearFrameBuffer();

    glClearColor(m_clear_color.red, m_clear_color.green, m_clear_color.blue, m_clear_color.alpha);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw frame buffer to screen

    mono::ITexturePtr frame_buffer_texture = m_frame_buffer->Texture();
    UseTexture(frame_buffer_texture.get());
    UseShader(m_screen_shader.get());
    ::DrawScreen(math::Quad(0.0f, 0.0f, 1.0f, 1.0f), math::Vector(1.0f, 1.0f), m_texture_shader.get());
}

void Renderer::DrawFrame()
{
    PrepareDraw();

    uint32_t draw_count = 0;

    for(const IDrawable* drawable : m_drawables)
    {
        // Make sure the drawable is visible
        const bool visible = Cull(drawable->BoundingBox());
        if(visible)
            drawable->Draw(*this);
        draw_count += visible;
    }

    EndDraw();
}

mono::FrameBuffer* Renderer::GetFramebuffer() const
{
    return m_frame_buffer.get();
}

void Renderer::AddDrawable(const IDrawable* drawable)
{
    m_drawables.push_back(drawable);
}

void Renderer::DrawText(int font_id, const char* text, const math::Vector& pos, bool center, const mono::Color::RGBA& color) const
{
    const mono::ITexturePtr& texture = mono::GetFontTexture(font_id);
    if(!texture)
        return;

    TextDefinition def = mono::GenerateVertexDataFromString(font_id, text, pos, center);
    def.color = color;

    UseTexture(texture.get());
    UseShader(m_texture_shader.get());

    ::DrawText(def, m_texture_shader.get());
}

void Renderer::DrawSprite(const ISprite& sprite) const
{
    const mono::ITexture* texture = sprite.GetTexture();
    const SpriteFrame& current_frame = sprite.GetCurrentFrame();

    UseShader(m_sprite_shader.get());
    SpriteShader::SetShade(m_sprite_shader.get(), sprite.GetShade());

    const uint32_t sprite_properties = sprite.GetProperties();
    SpriteShader::SetWindSway(m_sprite_shader.get(), sprite_properties & mono::SpriteProperty::WIND_SWAY);

    const bool flash_sprite = sprite.ShouldFlashSprite();
    SpriteShader::SetFlashSprite(m_sprite_shader.get(), flash_sprite);

    DrawSprite(
        current_frame.uv_upper_left,
        current_frame.uv_lower_right,
        current_frame.size,
        current_frame.center_offset,
        texture);
}

void Renderer::DrawSprite(
    const math::Vector& uv_upper_left,
    const math::Vector& uv_lower_right,
    const math::Vector& size,
    const math::Vector& offset,
    const ITexture* texture) const
{
    UseTexture(texture);
    UseShader(m_sprite_shader.get());
    ::DrawSprite(uv_upper_left, uv_lower_right, size, offset, m_sprite_shader.get());

    PROCESS_GL_ERRORS();
}

void Renderer::DrawPoints(const std::vector<math::Vector>& points, const mono::Color::RGBA& color, float size) const
{
    UseShader(m_color_shader.get());
    ::DrawPoints(points, color, size, m_color_shader.get());
}

void Renderer::DrawLines(const std::vector<math::Vector>& line_points, const mono::Color::RGBA& color, float width) const
{
    UseShader(m_color_shader.get());
    ::DrawLines(line_points, color, width, m_color_shader.get());
}

void Renderer::DrawPolyline(const std::vector<math::Vector>& line_points, const mono::Color::RGBA& color, float width) const
{
    UseShader(m_color_shader.get());
    ::DrawLine(line_points, color, width, m_color_shader.get());
}

void Renderer::DrawClosedPolyline(const std::vector<math::Vector>& line_points, const mono::Color::RGBA& color, float width) const
{
    UseShader(m_color_shader.get());
    ::DrawClosedLine(line_points, color, width, m_color_shader.get());
}

void Renderer::DrawQuad(const math::Quad& quad, const mono::Color::RGBA& color, float width) const
{
    UseShader(m_color_shader.get());
    ::DrawQuad(quad, color, width, m_color_shader.get());
}

void Renderer::DrawFilledQuad(const math::Quad& quad, const mono::Color::RGBA& color) const
{
    UseShader(m_color_shader.get());
    ::DrawFilledQuad(quad, color, m_color_shader.get());
}

void Renderer::DrawCircle(const math::Vector& pos, float radie, int segments, float line_width, const mono::Color::RGBA& color) const
{
    UseShader(m_color_shader.get());
    ::DrawCircle(pos, radie, segments, line_width, color, m_color_shader.get());
}

void Renderer::DrawFilledCircle(const math::Vector& pos, const math::Vector& size, int segments, const mono::Color::RGBA& color) const
{
    UseShader(m_color_shader.get());
    ::DrawFilledCircle(pos, size, segments, color, m_color_shader.get());
}

void Renderer::DrawGeometry(const std::vector<math::Vector>& vertices, const std::vector<math::Vector>& texture_coordinates, const std::vector<uint16_t>& indices, const ITexture* texture)
{
    UseTexture(texture);
    UseShader(m_texture_shader.get());
    ::DrawTexturedGeometry(vertices, texture_coordinates, indices, m_texture_shader.get());
}

void Renderer::DrawGeometry(
    const IRenderBuffer* vertices,
    const IRenderBuffer* texture_coordinates,
    size_t offset,
    size_t count,
    const ITexture* texture)
{
    UseTexture(texture);
    UseShader(m_texture_shader.get());
    ::DrawTexturedGeometry(vertices, texture_coordinates, offset, count, m_texture_shader.get());
}

void Renderer::DrawParticlePoints(
    const IRenderBuffer* position,
    const IRenderBuffer* rotation,
    const IRenderBuffer* color,
    const IRenderBuffer* point_size,
    const ITexture* texture,
    BlendMode blend_mode,
    size_t count)
{
    UseTexture(texture);
    UseShader(m_point_sprite_shader.get());
    ::DrawParticlePoints(position, rotation, color, point_size, count, blend_mode, m_point_sprite_shader.get());
}

void Renderer::DrawPolyline(const IRenderBuffer* vertices, const IRenderBuffer* colors, size_t offset, size_t count)
{
    UseShader(m_color_shader.get());
    ::DrawPolyline(vertices, colors, offset, count, 2.0f, m_color_shader.get());
}

void Renderer::DrawTrianges(
    const IRenderBuffer* vertices, const IRenderBuffer* colors, const IElementBuffer* indices, size_t count) const
{
    UseShader(m_color_shader.get());
    ::DrawTrianges(vertices, colors, indices, count, m_color_shader.get());
}

void Renderer::SetClearColor(const mono::Color::RGBA& color)
{
    m_clear_color = color;
}

void Renderer::UseShader(IShader* shader) const
{
    const uint32_t id = shader->GetShaderId();
    if(id != m_current_shader_id)
    {
        shader->Use();
        m_current_shader_id = id;
    }

    shader->SetTime(float(m_timestamp) / 1000.0f, float(m_delta_time_ms) / 1000.0f);

    if(!m_projection_stack.empty() && !m_view_stack.empty() && !m_model_stack.empty())
    {
        const math::Matrix& projection = m_projection_stack.top();
        const math::Matrix& view = m_view_stack.top();
        const math::Matrix& model = m_model_stack.top();
        shader->SetProjectionViewModel(projection, view, model);
    }

    PROCESS_GL_ERRORS();
}

void Renderer::UseTexture(const ITexture* texture) const
{
    const uint32_t id = texture->Id();
    if(id != m_current_texture_id)
    {
        texture->Use();
        m_current_texture_id = id;
    }

    PROCESS_GL_ERRORS();
}

void Renderer::ClearTexture()
{
    m_current_texture_id = -1;
    mono::ClearTexture();
}

const math::Matrix& Renderer::GetTransform() const
{
    return m_model_stack.top();
}

void Renderer::PushNewTransform(const math::Matrix& transform)
{
    m_model_stack.push(transform);
}

void Renderer::PopTransform()
{
    m_model_stack.pop();
}

void Renderer::PushNewProjection(const math::Matrix& projection)
{
    m_projection_stack.push(projection);
}

void Renderer::PopProjection()
{
    m_projection_stack.pop();
}

void Renderer::PushNewViewTransform(const math::Matrix& transform)
{
    m_view_stack.push(transform);
}

void Renderer::PopViewTransform()
{
    m_view_stack.pop();
}

mono::IShader* Renderer::GetSpriteShader()
{
    return m_sprite_shader.get();
}

mono::IShader* Renderer::GetScreenShader()
{
    return m_screen_shader.get();
}

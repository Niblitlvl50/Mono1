
#include "RendererSokol.h"

#include "IDrawable.h"
#include "Math/MathFunctions.h"
#include "RenderBuffer/IRenderBuffer.h"
#include "System/System.h"

#include "Rendering/RenderSystem.h"
#include "Rendering/Pipeline/ColorPipeline.h"
#include "Rendering/Pipeline/ParticlePointPipeline.h"
#include "Rendering/Pipeline/TexturePipeline.h"
#include "Rendering/Pipeline/ScreenPipeline.h"
#include "Rendering/Pipeline/SpritePipeline.h"

#include "Rendering/RenderBuffer/BufferFactory.h"
#include "Rendering/Texture/ITextureFactory.h"
#include "Rendering/Sprite/ISprite.h"
#include "Rendering/Sprite/SpriteProperties.h"

#include "Text/TextFunctions.h"

#define SOKOL_IMGUI_NO_SOKOL_APP
#include "sokol/sokol_imgui.h"

using namespace mono;

RendererSokol::RendererSokol()
    : m_offscreen_pass{}
    , m_clear_color(0.7f, 0.7f, 0.7f, 1.0f)
{
    m_color_points_pipeline = mono::ColorPipeline::MakePointsPipeline();
    m_color_lines_pipeline = mono::ColorPipeline::MakeLinesPipeline();
    m_color_line_strip_pipeline = mono::ColorPipeline::MakeLineStripPipeline();
    m_color_triangles_pipeline = mono::ColorPipeline::MakeTrianglesPipeline();

    m_particle_pipeline = mono::ParticlePointPipeline::MakePipeline();
    m_texture_pipeline = mono::TexturePipeline::MakePipeline();
    m_sprite_pipeline = mono::SpritePipeline::MakePipeline();
    m_screen_pipeline = mono::ScreenPipeline::MakePipeline();

    const math::Vector vertices[] = { {-1.0f, -1.0f}, {-1.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, -1.0f} };
    const math::Vector uv_coordinates[] = { {0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f} };
    const uint16_t indices[] = { 0, 1, 2, 0, 2, 3, };

    m_screen_vertices = CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 2, std::size(vertices), vertices);
    m_screen_uv = CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 2, std::size(uv_coordinates), uv_coordinates);
    m_screen_indices = CreateElementBuffer(BufferType::STATIC, std::size(indices), indices);

    m_temp_vertices = CreateRenderBuffer(BufferType::DYNAMIC, BufferData::FLOAT, 2, 200, nullptr);
    m_temp_uv_coords = CreateRenderBuffer(BufferType::DYNAMIC, BufferData::FLOAT, 2, 200, nullptr);
    m_temp_colors = CreateRenderBuffer(BufferType::DYNAMIC, BufferData::FLOAT, 4, 200, nullptr);
    m_temp_heights = CreateRenderBuffer(BufferType::DYNAMIC, BufferData::FLOAT, 1, 200, nullptr);

    m_temp_indices = CreateElementBuffer(BufferType::DYNAMIC, 200, nullptr);
}

RendererSokol::~RendererSokol()
{
    sg_destroy_pass(m_offscreen_pass.pass_handle);
}

void RendererSokol::SetWindowSize(const math::Vector& window_size)
{
    m_window_size = window_size;
}

void RendererSokol::SetDrawableSize(const math::Vector& drawable_size)
{
    m_drawable_size = drawable_size;
}

void RendererSokol::SetViewport(const math::Quad& viewport)
{
    m_viewport = viewport;
}

void RendererSokol::SetDeltaAndTimestamp(uint32_t delta_ms, uint32_t timestamp)
{
    m_delta_time_ms = delta_ms;
    m_timestamp = timestamp;
}

void RendererSokol::MakeOrUpdateOffscreenPass(RendererSokol::OffscreenPassData& offscreen_pass) const
{
    if(offscreen_pass.image_size == m_drawable_size)
        return;

    if(offscreen_pass.pass_handle.id != 0)
    {
        sg_destroy_pass(offscreen_pass.pass_handle);
    }

    sg_image_desc offscreen_image_desc = {};
    offscreen_image_desc.render_target = true;
    offscreen_image_desc.width = m_drawable_size.x;
    offscreen_image_desc.height = m_drawable_size.y;
    offscreen_image_desc.usage = SG_USAGE_DYNAMIC;
    sg_image image_handle = sg_make_image(offscreen_image_desc);

    sg_pass_desc pass_desc = {};
    pass_desc.color_attachments[0].image = image_handle;
    pass_desc.color_attachments[0].mip_level = 0;
    pass_desc.color_attachments[0].slice = 0;
    sg_pass pass_handle = sg_make_pass(&pass_desc);

    offscreen_pass.image_size = m_drawable_size;
    offscreen_pass.offscreen_texture = mono::GetTextureFactory()->CreateFromNativeHandle(image_handle.id);
    offscreen_pass.pass_handle = pass_handle;

    const sg_resource_state state = sg_query_pass_state(offscreen_pass.pass_handle);
    if(state != SG_RESOURCESTATE_VALID)
        System::Log("Failed to create render pass.\n");
}

void RendererSokol::PrepareDraw()
{
    m_projection_stack = {};
    m_view_stack = {};
    m_model_stack = {};

    const float viewport_width = math::Width(m_viewport);
    const float ratio = m_window_size.x / m_window_size.y;
    m_projection_stack.push(math::Ortho(0.0f, viewport_width, 0.0f, viewport_width / ratio, -10.0f, 10.0f));

    math::Matrix view_transform;
    math::Translate(view_transform, -m_viewport.mA);
    m_view_stack.push(view_transform);

    m_model_stack.push(math::Matrix()); // Push identity

    sg_apply_viewport(0, 0, m_drawable_size.x, m_drawable_size.y, false);

    MakeOrUpdateOffscreenPass(m_offscreen_pass);

    sg_pass_action offscreen_pass_action = {};
    offscreen_pass_action.colors[0].action = SG_ACTION_CLEAR;
    offscreen_pass_action.colors[0].val[0] = m_clear_color.red;
    offscreen_pass_action.colors[0].val[1] = m_clear_color.green;
    offscreen_pass_action.colors[0].val[2] = m_clear_color.blue;
    offscreen_pass_action.colors[0].val[3] = m_clear_color.alpha;
    sg_begin_pass(m_offscreen_pass.pass_handle, &offscreen_pass_action);

    const double delta_time_s = double(m_delta_time_ms) / 1000.0;
    simgui_new_frame(m_window_size.x, m_window_size.y, delta_time_s);
}

void RendererSokol::EndDraw()
{
    simgui_render();
    sg_end_pass(); // End offscreen render pass

    // Clear all the stuff once the frame has been drawn
    m_drawables.clear();

    sg_pass_action default_pass_action = {};
    sg_begin_default_pass(default_pass_action, m_drawable_size.x, m_drawable_size.y);

    ScreenPipeline::Apply(
        m_screen_pipeline.get(),
        m_screen_vertices.get(),
        m_screen_uv.get(),
        m_screen_indices.get(),
        m_offscreen_pass.offscreen_texture.get());
    ScreenPipeline::FadeCorners(false);
    ScreenPipeline::InvertColors(false);
    sg_draw(0, 6, 1);

    sg_end_pass(); // End default pass
    sg_commit();
}

void RendererSokol::DrawFrame()
{
    PrepareDraw();

    for(const IDrawable* drawable : m_drawables)
    {
        const bool visible = Cull(drawable->BoundingBox());
        if(visible)
            drawable->Draw(*this);
    }

    EndDraw();
}

void RendererSokol::AddDrawable(const IDrawable* drawable)
{
    m_drawables.push_back(drawable);
}

void RendererSokol::RenderText(
    int font_id,
    const char* text,
    const math::Vector& pos,
    bool center,
    const mono::Color::RGBA& color) const
{
    const mono::ITexturePtr& texture = mono::GetFontTexture(font_id);
    if(!texture)
        return;

    const TextDefinition& def = mono::GenerateVertexDataFromString(font_id, text, pos, center);

    m_temp_vertices->UpdateData(def.vertices.data(), 0, def.vertices.size());
    m_temp_uv_coords->UpdateData(def.texcoords.data(), 0, def.texcoords.size());
    m_temp_indices->UpdateData(def.indices.data(), 0, def.indices.size());

    TexturePipeline::Apply(
        m_texture_pipeline.get(), m_temp_vertices.get(), m_temp_uv_coords.get(), m_temp_indices.get(), texture.get());
    TexturePipeline::SetTransforms(m_projection_stack.top(), m_view_stack.top(), m_model_stack.top());
    TexturePipeline::SetIsAlpha(true);
    TexturePipeline::SetShade(color);

    sg_draw(0, def.indices.size(), 1);
}

void RendererSokol::DrawSprite(const ISprite& sprite) const
{
    const mono::ITexture* texture = sprite.GetTexture();
    const SpriteFrame& current_frame = sprite.GetCurrentFrame();

    const uint32_t sprite_properties = sprite.GetProperties();
    const bool flash_sprite = sprite.ShouldFlashSprite();
    const mono::Color::RGBA& shade = sprite.GetShade();

   const math::Vector& sprite_half_size = current_frame.size / 2.0f;

    const math::Vector vertices[] = {
        math::Vector(-sprite_half_size.x, -sprite_half_size.y) + current_frame.center_offset,
        math::Vector(-sprite_half_size.x,  sprite_half_size.y) + current_frame.center_offset,
        math::Vector( sprite_half_size.x,  sprite_half_size.y) + current_frame.center_offset,
        math::Vector( sprite_half_size.x, -sprite_half_size.y) + current_frame.center_offset,
    };

    const math::Vector uv_coords[] = {
        { current_frame.uv_upper_left.x, current_frame.uv_upper_left.y },
        { current_frame.uv_upper_left.x, current_frame.uv_lower_right.y },
        { current_frame.uv_lower_right.x, current_frame.uv_lower_right.y },
        { current_frame.uv_lower_right.x, current_frame.uv_upper_left.y },
    };

    const float height_values[] = {
        0.0f, current_frame.size.y, current_frame.size.y, 0.0f
    };

    constexpr uint16_t indices[] = {
        0, 1, 2, 0, 2, 3
    };

    m_temp_vertices->UpdateData(vertices, 0, std::size(vertices));
    m_temp_uv_coords->UpdateData(uv_coords, 0, std::size(uv_coords));
    m_temp_heights->UpdateData(height_values, 0, std::size(height_values));
    m_temp_indices->UpdateData(indices, 0, std::size(indices));

    SpritePipeline::Apply(
        m_sprite_pipeline.get(),
        m_temp_vertices.get(),
        m_temp_uv_coords.get(),
        m_temp_heights.get(),
        m_temp_indices.get(),
        texture);

    SpritePipeline::SetTime(float(m_timestamp) / 1000.0f, float(m_delta_time_ms) / 1000.0f);
    SpritePipeline::SetTransforms(m_projection_stack.top(), m_view_stack.top(), m_model_stack.top());

    SpritePipeline::SetWindSway(sprite_properties & mono::SpriteProperty::WIND_SWAY);
    SpritePipeline::SetShade(shade);
    SpritePipeline::SetFlashSprite(flash_sprite);

    sg_draw(0, 6, 1);
}

void RendererSokol::DrawSprite(
    const math::Vector& uv_upper_left,
    const math::Vector& uv_lower_right,
    const math::Vector& size,
    const math::Vector& offset,
    const ITexture* texture) const
{}

void RendererSokol::DrawPoints(const std::vector<math::Vector>& points, const mono::Color::RGBA& color, float point_size) const
{
    std::vector<mono::Color::RGBA> colors(points.size());
    std::fill(colors.begin(), colors.end(), color);

    m_temp_vertices->UpdateData(points.data(), 0, points.size());
    m_temp_colors->UpdateData(colors.data(), 0, colors.size());

    ColorPipeline::Apply(m_color_points_pipeline.get(), m_temp_vertices.get(), m_temp_colors.get());
    ColorPipeline::SetTransforms(m_projection_stack.top(), m_view_stack.top(), m_model_stack.top());
    ColorPipeline::SetPointSize(point_size);

    sg_draw(0, points.size(), 1);
}

void RendererSokol::DrawLines(const std::vector<math::Vector>& line_points, const mono::Color::RGBA& color, float width) const
{
    std::vector<mono::Color::RGBA> colors(line_points.size());
    std::fill(colors.begin(), colors.end(), color);

    m_temp_vertices->UpdateData(line_points.data(), 0, line_points.size());
    m_temp_colors->UpdateData(colors.data(), 0, colors.size());

    ColorPipeline::Apply(m_color_lines_pipeline.get(), m_temp_vertices.get(), m_temp_colors.get());
    ColorPipeline::SetTransforms(m_projection_stack.top(), m_view_stack.top(), m_model_stack.top());

    sg_draw(0, line_points.size(), 1);
}

void RendererSokol::DrawPolyline(const std::vector<math::Vector>& line_points, const mono::Color::RGBA& color, float line_width) const
{
    std::vector<mono::Color::RGBA> colors(line_points.size());
    std::fill(colors.begin(), colors.end(), color);

    m_temp_vertices->UpdateData(line_points.data(), 0, line_points.size());
    m_temp_colors->UpdateData(colors.data(), 0, colors.size());

    ColorPipeline::Apply(m_color_line_strip_pipeline.get(), m_temp_vertices.get(), m_temp_colors.get());
    ColorPipeline::SetTransforms(m_projection_stack.top(), m_view_stack.top(), m_model_stack.top());
    //ColorPipeline::SetLineWidth(line_width);

    sg_draw(0, line_points.size(), 1);
}

void RendererSokol::DrawClosedPolyline(const std::vector<math::Vector>& line_points, const mono::Color::RGBA& color, float width) const
{
    std::vector<math::Vector> local_line_points = line_points;
    local_line_points.push_back(local_line_points.front());
    DrawPolyline(local_line_points, color, width);
}

void RendererSokol::DrawCircle(const math::Vector& position, float radie, int segments, float line_width, const mono::Color::RGBA& color) const
{
    std::vector<math::Vector> vertices;
    vertices.reserve(segments);

    const float coef = 2.0f * math::PI() / float(segments);

    for(int index = 0; index < segments; ++index)
    {
        const float radians = index * coef;
        const float x = radie * std::cos(radians) + position.x;
        const float y = radie * std::sin(radians) + position.y;

        vertices.emplace_back(x, y);
    }

    DrawClosedPolyline(vertices, color, line_width);
}

void RendererSokol::DrawFilledCircle(const math::Vector& position, const math::Vector& size, int segments, const mono::Color::RGBA& color) const
{
    if((segments % 2) != 0)
        segments += 1;

    std::vector<math::Vector> vertices;
    vertices.reserve(segments +1);
    vertices.push_back(position);

    const uint16_t n_indices = segments * 3;
    std::vector<uint16_t> indices;
    indices.reserve(n_indices);

    const float coef = 2.0f * math::PI() / float(segments);

    for(int index = 0; index < segments; ++index)
    {
        const float radians = index * coef;
        const float x = size.x * std::cos(radians) + position.x;
        const float y = size.y * std::sin(radians) + position.y;
        vertices.emplace_back(x, y);

        indices.push_back(0);
        indices.push_back(index +1);
        indices.push_back(index +2);
    }

    indices.pop_back();
    indices.pop_back();

    indices.push_back(vertices.size() -1);
    indices.push_back(1);

    const std::vector<mono::Color::RGBA> colors(vertices.size(), color);

    m_temp_vertices->UpdateData(vertices.data(), 0, vertices.size());
    m_temp_colors->UpdateData(colors.data(), 0, colors.size());
    m_temp_indices->UpdateData(indices.data(), 0, indices.size());

    ColorPipeline::Apply(
        m_color_triangles_pipeline.get(), m_temp_vertices.get(), m_temp_colors.get(), m_temp_indices.get());
    ColorPipeline::SetTransforms(m_projection_stack.top(), m_view_stack.top(), m_model_stack.top());

    sg_draw(0, indices.size(), 1);
}

void RendererSokol::DrawQuad(const math::Quad& quad, const mono::Color::RGBA& color, float width) const
{
    const std::vector<math::Vector> line_points = {
        math::BottomLeft(quad),
        math::TopLeft(quad),
        math::TopRight(quad),
        math::BottomRight(quad),
        math::BottomLeft(quad),
    };

    DrawPolyline(line_points, color, width);
}

void RendererSokol::DrawFilledQuad(const math::Quad& quad, const mono::Color::RGBA& color) const
{
    const std::vector<math::Vector> line_points = {
        math::BottomLeft(quad),
        math::TopLeft(quad),
        math::TopRight(quad),
        math::BottomRight(quad),
    };

    std::vector<mono::Color::RGBA> colors(line_points.size());
    std::fill(colors.begin(), colors.end(), color);

    m_temp_vertices->UpdateData(line_points.data(), 0, line_points.size());
    m_temp_colors->UpdateData(colors.data(), 0, colors.size());

    constexpr uint16_t indices[] = { 0, 1, 2, 0, 2, 3};
    m_temp_indices->UpdateData(indices, 0, std::size(indices));

    ColorPipeline::Apply(m_color_triangles_pipeline.get(), m_temp_vertices.get(), m_temp_colors.get(), m_temp_indices.get());
    ColorPipeline::SetTransforms(m_projection_stack.top(), m_view_stack.top(), m_model_stack.top());

    sg_draw(0, std::size(indices), 1);
}

void RendererSokol::DrawGeometry(
    const IRenderBuffer* vertices,
    const IRenderBuffer* texture_coordinates,
    const IElementBuffer* indices,
    const ITexture* texture,
    uint32_t count)
{
    TexturePipeline::Apply(m_texture_pipeline.get(), vertices, texture_coordinates, indices, texture);
    //TexturePipeline::SetTime(m_texture_pipeline.get(), float(m_timestamp) / 1000.0f, float(m_delta_time_ms) / 1000.0f);
    TexturePipeline::SetTransforms(m_projection_stack.top(), m_view_stack.top(), m_model_stack.top());

    TexturePipeline::SetIsAlpha(false);
    TexturePipeline::SetShade(mono::Color::WHITE);

    sg_draw(0, count, 1);
}

void RendererSokol::DrawParticlePoints(
    const IRenderBuffer* position,
    const IRenderBuffer* rotation,
    const IRenderBuffer* color,
    const IRenderBuffer* point_size,
    const ITexture* texture,
    BlendMode blend_mode,
    uint32_t count)
{
    ParticlePointPipeline::Apply(m_particle_pipeline.get(), position, rotation, color, point_size, texture);
    //ParticlePointPipeline::SetTime(float(m_timestamp) / 1000.0f, float(m_delta_time_ms) / 1000.0f);
    ParticlePointPipeline::SetTransforms(m_projection_stack.top(), m_view_stack.top(), m_model_stack.top());

    sg_draw(0, count, 1);
}

void RendererSokol::DrawPolyline(
    const IRenderBuffer* vertices, const IRenderBuffer* colors, uint32_t offset, uint32_t count)
{
    ColorPipeline::Apply(m_color_line_strip_pipeline.get(), vertices, colors);
    ColorPipeline::SetTime(float(m_timestamp) / 1000.0f, float(m_delta_time_ms) / 1000.0f);
    ColorPipeline::SetTransforms(m_projection_stack.top(), m_view_stack.top(), m_model_stack.top());
    //ColorPipeline::SetLineWidth(10.0f);

    sg_draw(offset, count, 1);
}

void RendererSokol::DrawTrianges(
    const IRenderBuffer* vertices, const IRenderBuffer* colors, const IElementBuffer* indices, uint32_t count) const
{
    ColorPipeline::Apply(m_color_triangles_pipeline.get(), vertices, colors, indices);
    ColorPipeline::SetTime(float(m_timestamp) / 1000.0f, float(m_delta_time_ms) / 1000.0f);
    ColorPipeline::SetTransforms(m_projection_stack.top(), m_view_stack.top(), m_model_stack.top());

    sg_draw(0, count, 1);
}

void RendererSokol::SetClearColor(const mono::Color::RGBA& color)
{
    m_clear_color = color;
}

const math::Matrix& RendererSokol::GetTransform() const
{
    return m_model_stack.top();
}

void RendererSokol::PushNewTransform(const math::Matrix& transform)
{
    m_model_stack.push(transform);
}

void RendererSokol::PopTransform()
{
    m_model_stack.pop();
}

void RendererSokol::PushNewProjection(const math::Matrix& projection)
{
    m_projection_stack.push(projection);
}

void RendererSokol::PopProjection()
{
    m_projection_stack.pop();
}

void RendererSokol::PushNewViewTransform(const math::Matrix& transform)
{
    m_view_stack.push(transform);
}

void RendererSokol::PopViewTransform()
{
    m_view_stack.pop();
}

const math::Quad& RendererSokol::GetViewport() const
{
    return m_viewport;
}

bool RendererSokol::Cull(const math::Quad& world_bb) const
{
    return math::QuadOverlaps(m_viewport, world_bb);
}

uint32_t RendererSokol::GetDeltaTimeMS() const
{
    return m_delta_time_ms;
}

uint32_t RendererSokol::GetTimestamp() const
{
    return m_timestamp;
}


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
#include "Rendering/Pipeline/FogPipeline.h"

#include "Rendering/RenderBuffer/BufferFactory.h"
#include "Rendering/Texture/ITextureFactory.h"
#include "Rendering/Sprite/ISprite.h"
#include "Rendering/Sprite/SpriteProperties.h"

#include "Text/TextFunctions.h"

#define SOKOL_IMGUI_NO_SOKOL_APP
#include "sokol/sokol_imgui.h"

using namespace mono;

RendererSokol::RendererSokol()
    : m_offscreen_color_pass{}
    , m_offscreen_light_pass{}
    , m_clear_color(0.7f, 0.7f, 0.7f)
    , m_ambient_shade(1.0f, 1.0f, 1.0f)
    , m_screen_fade_alpha(1.0f)
{
    m_color_points_pipeline = mono::ColorPipeline::MakePointsPipeline();
    m_color_lines_pipeline = mono::ColorPipeline::MakeLinesPipeline();
    m_color_lines_indices_pipeline = mono::ColorPipeline::MakeLinesPipelineIndices();
    m_color_line_strip_pipeline = mono::ColorPipeline::MakeLineStripPipeline();
    m_color_line_strip_indices_pipeline = mono::ColorPipeline::MakeLineStripIndicesPipeline();
    m_color_triangles_pipeline = mono::ColorPipeline::MakeTrianglesPipeline();

    m_particle_pipeline_one = mono::ParticlePointPipeline::MakePipeline(mono::BlendMode::ONE);
    m_particle_pipeline_sa = mono::ParticlePointPipeline::MakePipeline(mono::BlendMode::SOURCE_ALPHA);

    m_texture_pipeline = mono::TexturePipeline::MakePipeline();
    m_texture_annotation_pipeline = mono::TexturePipeline::MakeAnnotationPipeline();
    m_texture_pipeline_color = mono::TexturePipeline::MakeVertexColorPipeline();
    m_sprite_pipeline = mono::SpritePipeline::MakePipeline();
    m_fog_pipeline = mono::FogPipeline::MakePipeline();
    m_screen_pipeline = mono::ScreenPipeline::MakePipeline();

    constexpr math::Vector vertices[] = { {-1.0f, -1.0f}, {-1.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, -1.0f} };
    constexpr math::Vector uv_coordinates[] = { {0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f} };
    constexpr uint16_t indices[] = { 0, 1, 2, 0, 2, 3, };

    m_screen_vertices = CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 2, std::size(vertices), vertices);
    m_screen_uv = CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 2, std::size(uv_coordinates), uv_coordinates);
    m_screen_indices = CreateElementBuffer(BufferType::STATIC, std::size(indices), indices);

    const char* light_mask_texture = mono::LightMaskTexture();
    if(light_mask_texture)
        m_light_mask_texture = mono::GetTextureFactory()->CreateTexture(light_mask_texture);
}

RendererSokol::~RendererSokol()
{
    sg_destroy_pass(m_offscreen_color_pass.pass_handle);
    sg_destroy_pass(m_offscreen_light_pass.pass_handle);
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

void RendererSokol::SetDeltaAndTimestamp(uint32_t delta_ms, float delta_s, uint32_t timestamp)
{
    m_delta_time_ms = delta_ms;
    m_delta_time_s = delta_s;
    m_timestamp = timestamp;
}

void RendererSokol::MakeOrUpdateOffscreenPass(RendererSokol::OffscreenPassData& offscreen_pass) const
{
    if(offscreen_pass.image_size == m_drawable_size)
        return;

    if(offscreen_pass.pass_handle.id != 0)
        sg_destroy_pass(offscreen_pass.pass_handle);

    sg_image_desc offscreen_image_desc = {};
    offscreen_image_desc.render_target = true;
    offscreen_image_desc.width = m_drawable_size.x;
    offscreen_image_desc.height = m_drawable_size.y;
    sg_image image_handle = sg_make_image(offscreen_image_desc);

    sg_pass_desc pass_desc = {};
    pass_desc.color_attachments[0].image = image_handle;
    sg_pass pass_handle = sg_make_pass(&pass_desc);

    offscreen_pass.image_size = m_drawable_size;
    offscreen_pass.offscreen_texture = mono::GetTextureFactory()->CreateFromNativeHandle(image_handle.id);
    offscreen_pass.pass_handle = pass_handle;

    const sg_resource_state state = sg_query_pass_state(offscreen_pass.pass_handle);
    if(state != SG_RESOURCESTATE_VALID)
        System::Log("Failed to create render pass.");
}

void RendererSokol::DrawLights()
{
    sg_pass_action offscreen_light_pass_action = {};
    offscreen_light_pass_action.colors[0].action = SG_ACTION_CLEAR;

    offscreen_light_pass_action.colors[0].value.r = m_ambient_shade.red;
    offscreen_light_pass_action.colors[0].value.g = m_ambient_shade.green;
    offscreen_light_pass_action.colors[0].value.b = m_ambient_shade.blue;
    offscreen_light_pass_action.colors[0].value.a = m_ambient_shade.alpha;

    sg_begin_pass(m_offscreen_light_pass.pass_handle, &offscreen_light_pass_action);

    if(m_light_mask_texture && !m_lights.empty())
    {
        const uint32_t n_light_vertices = m_lights.size() * 4;
        const uint32_t n_light_indices = m_lights.size() * 3 * 2;

        std::vector<math::Vector> vertex_data;
        vertex_data.reserve(n_light_vertices);

        std::vector<math::Vector> uv_data;
        uv_data.reserve(n_light_vertices);

        std::vector<mono::Color::RGBA> color_data;
        color_data.reserve(n_light_vertices);

        std::vector<uint16_t> index_data;
        index_data.reserve(n_light_indices);

        for(const LightData& light : m_lights)
        {
            const uint32_t index_offset = vertex_data.size();

            vertex_data.push_back(light.position - math::Vector(-light.radius, -light.radius));
            vertex_data.push_back(light.position - math::Vector(-light.radius,  light.radius));
            vertex_data.push_back(light.position - math::Vector( light.radius,  light.radius));
            vertex_data.push_back(light.position - math::Vector( light.radius, -light.radius));

            uv_data.push_back(math::Vector(0.0f, 0.0f));
            uv_data.push_back(math::Vector(0.0f, 1.0f));
            uv_data.push_back(math::Vector(1.0f, 1.0f));
            uv_data.push_back(math::Vector(1.0f, 0.0f));

            color_data.push_back(light.shade);
            color_data.push_back(light.shade);
            color_data.push_back(light.shade);
            color_data.push_back(light.shade);

            index_data.push_back(index_offset + 0);
            index_data.push_back(index_offset + 1);
            index_data.push_back(index_offset + 2);
            index_data.push_back(index_offset + 0);
            index_data.push_back(index_offset + 2);
            index_data.push_back(index_offset + 3);
        }

        const auto vertex_buffer = mono::CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 2, n_light_vertices, vertex_data.data());
        const auto uv_buffer = mono::CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 2, n_light_vertices, uv_data.data());
        const auto color_buffer = mono::CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 4, n_light_vertices, color_data.data());
        const auto index_buffer = mono::CreateElementBuffer(BufferType::STATIC, n_light_indices, index_data.data());

        DrawGeometry(vertex_buffer.get(), uv_buffer.get(), color_buffer.get(), index_buffer.get(), m_light_mask_texture.get(), false, index_buffer->Size());
    }

    sg_end_pass(); // End offscreen light render pass
    m_lights.clear();
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

    MakeOrUpdateOffscreenPass(m_offscreen_color_pass);
    MakeOrUpdateOffscreenPass(m_offscreen_light_pass);

    sg_pass_action offscreen_pass_action = {};
    offscreen_pass_action.colors[0].action = SG_ACTION_CLEAR;
    offscreen_pass_action.colors[0].value.r = m_clear_color.red;
    offscreen_pass_action.colors[0].value.g = m_clear_color.green;
    offscreen_pass_action.colors[0].value.b = m_clear_color.blue;
    offscreen_pass_action.colors[0].value.a = m_clear_color.alpha;
    sg_begin_pass(m_offscreen_color_pass.pass_handle, &offscreen_pass_action);
    sg_apply_viewport(0, 0, m_drawable_size.x, m_drawable_size.y, false);

    simgui_new_frame(m_drawable_size.x, m_drawable_size.y, m_delta_time_s);
}

void RendererSokol::EndDraw()
{
    sg_end_pass(); // End offscreen color render pass

    DrawLights();

    sg_pass_action default_pass_action = {};
    sg_begin_default_pass(default_pass_action, m_drawable_size.x, m_drawable_size.y);

    ScreenPipeline::Apply(
        m_screen_pipeline.get(),
        m_screen_vertices.get(),
        m_screen_uv.get(),
        m_screen_indices.get(),
        m_offscreen_color_pass.offscreen_texture.get(),
        m_offscreen_light_pass.offscreen_texture.get());
    ScreenPipeline::FadeCorners(false);
    ScreenPipeline::InvertColors(false);
    ScreenPipeline::FadeScreenAlpha(m_screen_fade_alpha);

    sg_draw(0, 6, 1);

    for(const IDrawable* drawable : m_drawables[RenderPass::POST_LIGHTING])
    {
        const bool visible = Cull(drawable->BoundingBox());
        if(visible)
            drawable->Draw(*this);
    }

    simgui_render();

    sg_end_pass(); // End default pass
    sg_commit();

    // Clear all the stuff once the frame has been drawn
    for(uint32_t index = 0; index < RenderPass::N_RENDER_PASS; ++index)
        m_drawables[index].clear();
}

void RendererSokol::DrawFrame()
{
    PrepareDraw();

    for(const IDrawable* drawable : m_drawables[RenderPass::GENERAL])
    {
        const bool visible = Cull(drawable->BoundingBox());
        if(visible)
            drawable->Draw(*this);
    }

    EndDraw();
}

void RendererSokol::AddDrawable(const IDrawable* drawable, RenderPass render_pass)
{
    m_drawables[render_pass].push_back(drawable);
}

void RendererSokol::RenderText(
    int font_id,
    const char* text,
    const mono::Color::RGBA& color,
    mono::FontCentering center_flags) const
{
    if(std::strlen(text) == 0)
        return;

    const mono::ITexturePtr& texture = mono::GetFontTexture(font_id);
    if(!texture)
        return;

    const TextDefinition& def = mono::GenerateVertexDataFromString(font_id, text, center_flags);

    auto vertices = CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 2, def.vertices.size(), def.vertices.data());
    auto uv = CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 2, def.texcoords.size(), def.texcoords.data());
    auto indices = CreateElementBuffer(BufferType::STATIC, def.indices.size(), def.indices.data());

    RenderText(vertices.get(), uv.get(), indices.get(), texture.get(), color);
}

void RendererSokol::RenderText(
    const IRenderBuffer* vertices,
    const IRenderBuffer* uv,
    const IElementBuffer* indices,
    const ITexture* texture,
    const mono::Color::RGBA& color) const
{
    TexturePipeline::Apply(m_texture_pipeline.get(), vertices, uv, indices, texture);
    TexturePipeline::SetTransforms(m_projection_stack.top(), m_view_stack.top(), m_model_stack.top());
    TexturePipeline::SetIsAlpha(true);
    TexturePipeline::SetBlur(false);
    TexturePipeline::SetShade(color);

    sg_draw(0, indices->Size(), 1);
}

void RendererSokol::DrawSprite(
    const ISprite* sprite,
    const IRenderBuffer* vertices,
    const IRenderBuffer* offsets,
    const IRenderBuffer* uv_coordinates,
    const IRenderBuffer* uv_coordinates_flipped,
    const IRenderBuffer* height_values,
    const IElementBuffer* indices,
    const ITexture* texture,
    uint32_t buffer_offset) const
{
    SpritePipeline::Apply(
        m_sprite_pipeline.get(),
        vertices,
        offsets,
        uv_coordinates,
        uv_coordinates_flipped,
        height_values,
        indices,
        texture,
        buffer_offset);

    SpritePipeline::SetTime(float(m_timestamp) / 1000.0f, float(m_delta_time_ms) / 1000.0f);
    SpritePipeline::SetTransforms(m_projection_stack.top(), m_view_stack.top(), m_model_stack.top());

    const uint32_t sprite_properties = sprite->GetProperties();

    SpritePipeline::SetFlipSprite(
        sprite_properties & mono::SpriteProperty::FLIP_HORIZONTAL,
        sprite_properties & mono::SpriteProperty::FLIP_VERTICAL);
    SpritePipeline::SetWindSway(sprite_properties & mono::SpriteProperty::WIND_SWAY);
    SpritePipeline::SetShade(sprite->GetShade());
    SpritePipeline::SetFlashSprite(sprite->ShouldFlashSprite());

    sg_draw(0, 6, 1);
}

void RendererSokol::DrawFog(const IRenderBuffer* vertices, const IElementBuffer* indices, const ITexture* texture)
{
    FogPipeline::Apply(m_fog_pipeline.get(), vertices, indices, texture);
    FogPipeline::SetTime(float(m_timestamp) / 1000.0f, float(m_delta_time_ms) / 1000.0f);
    FogPipeline::SetTransforms(m_projection_stack.top(), m_view_stack.top(), m_model_stack.top());
    FogPipeline::SetShade(mono::Color::WHITE);

    sg_draw(0, indices->Size(), 1);
}

void RendererSokol::DrawPoints(const std::vector<math::Vector>& points, const mono::Color::RGBA& color, float point_size) const
{
    if(points.empty())
        return;

    const std::vector<mono::Color::RGBA> colors(points.size(), color);
    DrawPoints(points, colors, point_size);
}

void RendererSokol::DrawPoints(const std::vector<math::Vector>& points, const std::vector<mono::Color::RGBA>& colors, float point_size) const
{
    if(points.empty())
        return;

    auto vertices = CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 2, points.size(), points.data());
    auto color_buffer = CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 4, colors.size(), colors.data());

    ColorPipeline::Apply(m_color_points_pipeline.get(), vertices.get(), color_buffer.get());
    ColorPipeline::SetTransforms(m_projection_stack.top(), m_view_stack.top(), m_model_stack.top());
    ColorPipeline::SetPointSize(point_size);

    sg_draw(0, points.size(), 1);
}

void RendererSokol::DrawLines(const std::vector<math::Vector>& line_points, const mono::Color::RGBA& color, float line_width) const
{
    if(line_points.empty())
        return;

    const std::vector<mono::Color::RGBA> colors(line_points.size(), color);

    auto vertices = CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 2, line_points.size(), line_points.data());
    auto color_buffer = CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 4, colors.size(), colors.data());

    ColorPipeline::Apply(m_color_lines_pipeline.get(), vertices.get(), color_buffer.get());
    ColorPipeline::SetTransforms(m_projection_stack.top(), m_view_stack.top(), m_model_stack.top());
    ColorPipeline::SetLineWidth(line_width);

    sg_draw(0, line_points.size(), 1);
}

void RendererSokol::DrawPolyline(const std::vector<math::Vector>& line_points, const mono::Color::RGBA& color, float line_width) const
{
    if(line_points.empty())
        return;

    const std::vector<mono::Color::RGBA> colors(line_points.size(), color);

    auto vertices = CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 2, line_points.size(), line_points.data());
    auto color_buffer = CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 4, colors.size(), colors.data());

    ColorPipeline::Apply(m_color_line_strip_pipeline.get(), vertices.get(), color_buffer.get());
    ColorPipeline::SetTransforms(m_projection_stack.top(), m_view_stack.top(), m_model_stack.top());
    ColorPipeline::SetLineWidth(line_width);

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

    auto vertex_buffer = CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 2, vertices.size(), vertices.data());
    auto color_buffer = CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 4, colors.size(), colors.data());
    auto index_buffer = CreateElementBuffer(BufferType::STATIC, indices.size(), indices.data());

    ColorPipeline::Apply(
        m_color_triangles_pipeline.get(), vertex_buffer.get(), color_buffer.get(), index_buffer.get());
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

    const std::vector<mono::Color::RGBA> colors(line_points.size(), color);
    constexpr uint16_t indices[] = { 0, 1, 2, 0, 2, 3};

    auto vertex_buffer = CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 2, line_points.size(), line_points.data());
    auto color_buffer = CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 4, colors.size(), colors.data());
    auto index_buffer = CreateElementBuffer(BufferType::STATIC, std::size(indices), indices);

    ColorPipeline::Apply(m_color_triangles_pipeline.get(), vertex_buffer.get(), color_buffer.get(), index_buffer.get());
    ColorPipeline::SetTransforms(m_projection_stack.top(), m_view_stack.top(), m_model_stack.top());

    sg_draw(0, std::size(indices), 1);
}

void RendererSokol::DrawGeometry(
    const IRenderBuffer* vertices,
    const IRenderBuffer* texture_coordinates,
    const IElementBuffer* indices,
    const ITexture* texture,
    bool blur,
    uint32_t count)
{
    TexturePipeline::Apply(m_texture_pipeline.get(), vertices, texture_coordinates, indices, texture);
    //TexturePipeline::SetTime(m_texture_pipeline.get(), float(m_timestamp) / 1000.0f, float(m_delta_time_ms) / 1000.0f);
    TexturePipeline::SetTransforms(m_projection_stack.top(), m_view_stack.top(), m_model_stack.top());

    TexturePipeline::SetIsAlpha(false);
    TexturePipeline::SetBlur(blur);
    TexturePipeline::SetShade(mono::Color::WHITE);

    sg_draw(0, count, 1);
}

void RendererSokol::DrawGeometry(
    const IRenderBuffer* vertices,
    const IRenderBuffer* uv_coordinates,
    const IRenderBuffer* vertex_colors,
    const IElementBuffer* indices,
    const ITexture* texture,
    bool blur,
    uint32_t count)
{
    TexturePipeline::Apply(m_texture_pipeline_color.get(), vertices, uv_coordinates, vertex_colors, indices, texture);
    //TexturePipeline::SetTime(m_texture_pipeline.get(), float(m_timestamp) / 1000.0f, float(m_delta_time_ms) / 1000.0f);
    TexturePipeline::SetTransforms(m_projection_stack.top(), m_view_stack.top(), m_model_stack.top());

    TexturePipeline::SetIsAlpha(false);
    TexturePipeline::SetBlur(blur);
    //TexturePipeline::SetShade(mono::Color::WHITE);

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
    mono::IPipeline* pipeline = (blend_mode == mono::BlendMode::ONE) ? m_particle_pipeline_one.get() : m_particle_pipeline_sa.get();
    ParticlePointPipeline::Apply(pipeline, position, rotation, color, point_size, texture);
    //ParticlePointPipeline::SetTime(float(m_timestamp) / 1000.0f, float(m_delta_time_ms) / 1000.0f);
    ParticlePointPipeline::SetTransforms(m_projection_stack.top(), m_view_stack.top(), m_model_stack.top());

    sg_draw(0, count, 1);
}

void RendererSokol::DrawPoints(
    const IRenderBuffer* vertices, const IRenderBuffer* colors, float point_size, uint32_t offset, uint32_t count)
{
    ColorPipeline::Apply(m_color_points_pipeline.get(), vertices, colors);
    ColorPipeline::SetTransforms(m_projection_stack.top(), m_view_stack.top(), m_model_stack.top());
    ColorPipeline::SetPointSize(point_size);

    sg_draw(offset, count, 1);
}

void RendererSokol::DrawLines(
    const IRenderBuffer* vertices, const IRenderBuffer* colors, uint32_t offset, uint32_t count)
{
    ColorPipeline::Apply(m_color_lines_pipeline.get(), vertices, colors);
    ColorPipeline::SetTime(float(m_timestamp) / 1000.0f, float(m_delta_time_ms) / 1000.0f);
    ColorPipeline::SetTransforms(m_projection_stack.top(), m_view_stack.top(), m_model_stack.top());
    ColorPipeline::SetLineWidth(1.0f);

    sg_draw(offset, count, 1);
}

void RendererSokol::DrawLines(
    const IRenderBuffer* vertices, const IRenderBuffer* colors, const IElementBuffer* indices, uint32_t offset, uint32_t count)
{
    ColorPipeline::Apply(m_color_lines_indices_pipeline.get(), vertices, colors, indices);
    ColorPipeline::SetTime(float(m_timestamp) / 1000.0f, float(m_delta_time_ms) / 1000.0f);
    ColorPipeline::SetTransforms(m_projection_stack.top(), m_view_stack.top(), m_model_stack.top());
    ColorPipeline::SetLineWidth(1.0f);

    sg_draw(offset, count, 1);
}

void RendererSokol::DrawPolyline(
    const IRenderBuffer* vertices, const IRenderBuffer* colors, uint32_t offset, uint32_t count)
{
    ColorPipeline::Apply(m_color_line_strip_pipeline.get(), vertices, colors);
    ColorPipeline::SetTime(float(m_timestamp) / 1000.0f, float(m_delta_time_ms) / 1000.0f);
    ColorPipeline::SetTransforms(m_projection_stack.top(), m_view_stack.top(), m_model_stack.top());
    ColorPipeline::SetLineWidth(1.0f);

    sg_draw(offset, count, 1);
}

void RendererSokol::DrawPolyline(
    const IRenderBuffer* vertices, const IRenderBuffer* colors, const IElementBuffer* indices, uint32_t offset, uint32_t count)
{
    ColorPipeline::Apply(m_color_line_strip_indices_pipeline.get(), vertices, colors, indices);
    ColorPipeline::SetTime(float(m_timestamp) / 1000.0f, float(m_delta_time_ms) / 1000.0f);
    ColorPipeline::SetTransforms(m_projection_stack.top(), m_view_stack.top(), m_model_stack.top());
    ColorPipeline::SetLineWidth(1.0f);

    sg_draw(offset, count, 1);
}

void RendererSokol::DrawTrianges(
    const IRenderBuffer* vertices, const IRenderBuffer* colors, const IElementBuffer* indices, uint32_t offset, uint32_t count) const
{
    ColorPipeline::Apply(m_color_triangles_pipeline.get(), vertices, colors, indices);
    ColorPipeline::SetTime(float(m_timestamp) / 1000.0f, float(m_delta_time_ms) / 1000.0f);
    ColorPipeline::SetTransforms(m_projection_stack.top(), m_view_stack.top(), m_model_stack.top());

    sg_draw(offset, count, 1);
}

void RendererSokol::DrawAnnotatedTrianges(
    const IRenderBuffer* vertices,
    const IRenderBuffer* annotations,
    const IElementBuffer* indices,
    const ITexture* texture,
    const mono::Color::RGBA& shade,
    uint32_t offset,
    uint32_t count) const
{
    TexturePipeline::Apply(m_texture_annotation_pipeline.get(), vertices, annotations, indices, texture);
    //TexturePipeline::SetTime(float(m_timestamp) / 1000.0f, float(m_delta_time_ms) / 1000.0f);
    TexturePipeline::SetTransforms(m_projection_stack.top(), m_view_stack.top(), m_model_stack.top());
    TexturePipeline::SetIsAlpha(false);
    TexturePipeline::SetBlur(false);
    TexturePipeline::SetShade(shade);

    sg_draw(offset, count, 1);
}

void RendererSokol::AddLight(const math::Vector& world_position, float radius, const mono::Color::RGBA& shade)
{
    m_lights.push_back({ world_position, radius, shade });
}

void RendererSokol::SetClearColor(const mono::Color::RGBA& color)
{
    m_clear_color = color;
}

void RendererSokol::SetAmbientShade(const mono::Color::RGBA& ambient_shade)
{
    m_ambient_shade = ambient_shade;
}

void RendererSokol::SetScreenFadeAlpha(float alpha)
{
    m_screen_fade_alpha = std::clamp(alpha, 0.0f, 1.0f);
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

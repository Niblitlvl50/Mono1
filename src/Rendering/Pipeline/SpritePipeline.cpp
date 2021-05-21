
#include "SpritePipeline.h"
#include "Impl/PipelineImpl.h"

#include "Math/Matrix.h"
#include "Rendering/Color.h"
#include "Rendering/RenderBuffer/IRenderBuffer.h"
#include "Rendering/Texture/ITexture.h"
#include "System/System.h"

#include "sokol/sokol_gfx.h"

namespace
{
    constexpr const char* vertex_source = R"(
        #version 330

        struct TimeInput
        {
            float total_time;
            float delta_time;
        };

        struct TransformInput
        {
            mat4 projection;
            mat4 view;
            mat4 model;
        };

        struct FlipSpriteInput
        {
            float flip_horizontal;
            float flip_vertical;
        };

        uniform TimeInput time_input;
        uniform TransformInput transform_input;
        uniform FlipSpriteInput flip_input;
        uniform float wind_sway_enabled;

        layout (location = 0) in vec2 vertex_position;
        layout (location = 1) in vec2 vertex_offset;
        layout (location = 2) in vec2 uv;
        layout (location = 3) in vec2 uv_flipped;
        layout (location = 4) in float vertex_height;

        out vec2 v_texture_coord;

        void main()
        {
            vec2 wind_sway_offset = vec2(0.0, 0.0);
            if(wind_sway_enabled != 0.0)
            {
                // Use world x as noise parameter to make some difference.
                float noise = abs(noise1(transform_input.model[3][0]));
                wind_sway_offset.x = sin(time_input.total_time * noise * 3.0) * (vertex_height * 0.025);
            }

            gl_Position =
                transform_input.projection *
                transform_input.view *
                transform_input.model *
                vec4(vertex_position + vertex_offset + wind_sway_offset, 0.0, 1.0);

            vec2 local_texture_coord = uv;
            if(flip_input.flip_vertical != 0.0)
                local_texture_coord.y = uv_flipped.y;
            if(flip_input.flip_horizontal != 0.0)
                local_texture_coord.x = uv_flipped.x;
            v_texture_coord = local_texture_coord;
        }
    )";

    constexpr const char* fragment_source = R"(
        #version 330

        uniform sampler2D sampler;
        uniform vec4 color_shade;
        uniform float flash_sprite;

        in vec2 v_texture_coord;
        out vec4 frag_color;

        void main()
        {
            vec4 color = texture(sampler, v_texture_coord) * color_shade;
            if(flash_sprite != 0.0)
                color.rgb = vec3(1.0);

            frag_color = color;
        }
    )";


    constexpr int U_VS_TIME_BLOCK = 0;
    constexpr int U_VS_TRANSFORM_BLOCK = 1;
    constexpr int U_VS_FLIP_SPRITE_BLOCK = 2;
    constexpr int U_VS_WIND_SWAY_BLOCK = 3;

    constexpr int U_FS_COLOR_SHADE_BLOCK = 0;
    constexpr int U_FS_FLASH_BLOCK = 1;

    constexpr int ATTR_POSITION = 0;
    constexpr int ATTR_POSITION_OFFSET = 1;
    constexpr int ATTR_UV = 2;
    constexpr int ATTR_UV_FLIPPED = 3;
    constexpr int ATTR_HEIGHT = 4;
}

using namespace mono;

mono::IPipelinePtr SpritePipeline::MakePipeline()
{
    sg_shader_desc shader_desc = {};
    shader_desc.vs.source = vertex_source;
    shader_desc.attrs[ATTR_POSITION].name = "vertex_position";
    shader_desc.attrs[ATTR_POSITION_OFFSET].name = "vertex_offset";
    shader_desc.attrs[ATTR_UV].name = "uv";
    shader_desc.attrs[ATTR_UV_FLIPPED].name = "uv_flipped";
    shader_desc.attrs[ATTR_HEIGHT].name = "vertex_height";

    shader_desc.vs.uniform_blocks[U_VS_TIME_BLOCK].size = sizeof(float) * 2;
    shader_desc.vs.uniform_blocks[U_VS_TIME_BLOCK].uniforms[0].name = "time_input.total_time";
    shader_desc.vs.uniform_blocks[U_VS_TIME_BLOCK].uniforms[0].type = SG_UNIFORMTYPE_FLOAT;
    shader_desc.vs.uniform_blocks[U_VS_TIME_BLOCK].uniforms[1].name = "time_input.delta_time";
    shader_desc.vs.uniform_blocks[U_VS_TIME_BLOCK].uniforms[1].type = SG_UNIFORMTYPE_FLOAT;

    shader_desc.vs.uniform_blocks[U_VS_TRANSFORM_BLOCK].size = sizeof(math::Matrix) * 3;
    shader_desc.vs.uniform_blocks[U_VS_TRANSFORM_BLOCK].uniforms[0].name = "transform_input.projection";
    shader_desc.vs.uniform_blocks[U_VS_TRANSFORM_BLOCK].uniforms[0].type = SG_UNIFORMTYPE_MAT4;
    shader_desc.vs.uniform_blocks[U_VS_TRANSFORM_BLOCK].uniforms[1].name = "transform_input.view";
    shader_desc.vs.uniform_blocks[U_VS_TRANSFORM_BLOCK].uniforms[1].type = SG_UNIFORMTYPE_MAT4;
    shader_desc.vs.uniform_blocks[U_VS_TRANSFORM_BLOCK].uniforms[2].name = "transform_input.model";
    shader_desc.vs.uniform_blocks[U_VS_TRANSFORM_BLOCK].uniforms[2].type = SG_UNIFORMTYPE_MAT4;

    shader_desc.vs.uniform_blocks[U_VS_FLIP_SPRITE_BLOCK].size = sizeof(float) * 2;
    shader_desc.vs.uniform_blocks[U_VS_FLIP_SPRITE_BLOCK].uniforms[0].name = "flip_input.flip_horizontal";
    shader_desc.vs.uniform_blocks[U_VS_FLIP_SPRITE_BLOCK].uniforms[0].type = SG_UNIFORMTYPE_FLOAT;
    shader_desc.vs.uniform_blocks[U_VS_FLIP_SPRITE_BLOCK].uniforms[1].name = "flip_input.flip_vertical";
    shader_desc.vs.uniform_blocks[U_VS_FLIP_SPRITE_BLOCK].uniforms[1].type = SG_UNIFORMTYPE_FLOAT;

    shader_desc.vs.uniform_blocks[U_VS_WIND_SWAY_BLOCK].size = sizeof(float);
    shader_desc.vs.uniform_blocks[U_VS_WIND_SWAY_BLOCK].uniforms[0].name = "wind_sway_enabled";
    shader_desc.vs.uniform_blocks[U_VS_WIND_SWAY_BLOCK].uniforms[0].type = SG_UNIFORMTYPE_FLOAT;

    shader_desc.fs.source = fragment_source;

    shader_desc.fs.images[0].name = "sampler";
    shader_desc.fs.images[0].type = SG_IMAGETYPE_2D;
    shader_desc.fs.images[0].sampler_type = SG_SAMPLERTYPE_FLOAT;

    shader_desc.fs.uniform_blocks[U_FS_COLOR_SHADE_BLOCK].size = sizeof(mono::Color::RGBA);
    shader_desc.fs.uniform_blocks[U_FS_COLOR_SHADE_BLOCK].uniforms[0].name = "color_shade";
    shader_desc.fs.uniform_blocks[U_FS_COLOR_SHADE_BLOCK].uniforms[0].type = SG_UNIFORMTYPE_FLOAT4;

    shader_desc.fs.uniform_blocks[U_FS_FLASH_BLOCK].size = sizeof(float);
    shader_desc.fs.uniform_blocks[U_FS_FLASH_BLOCK].uniforms[0].name = "flash_sprite";
    shader_desc.fs.uniform_blocks[U_FS_FLASH_BLOCK].uniforms[0].type = SG_UNIFORMTYPE_FLOAT;

    sg_shader shader_handle = sg_make_shader(&shader_desc);

    const sg_resource_state state = sg_query_shader_state(shader_handle);
    if(state != SG_RESOURCESTATE_VALID)
        System::Log("Failed to create sprite shader.");

    sg_pipeline_desc pipeline_desc = {};
    pipeline_desc.primitive_type = SG_PRIMITIVETYPE_TRIANGLES;
    pipeline_desc.index_type = SG_INDEXTYPE_UINT16;
    pipeline_desc.shader = shader_handle;

    pipeline_desc.layout.attrs[ATTR_POSITION].format = SG_VERTEXFORMAT_FLOAT2;
    pipeline_desc.layout.attrs[ATTR_POSITION].buffer_index = ATTR_POSITION;

    pipeline_desc.layout.attrs[ATTR_POSITION_OFFSET].format = SG_VERTEXFORMAT_FLOAT2;
    pipeline_desc.layout.attrs[ATTR_POSITION_OFFSET].buffer_index = ATTR_POSITION_OFFSET;

    pipeline_desc.layout.attrs[ATTR_UV].format = SG_VERTEXFORMAT_FLOAT2;
    pipeline_desc.layout.attrs[ATTR_UV].buffer_index = ATTR_UV;

    pipeline_desc.layout.attrs[ATTR_UV_FLIPPED].format = SG_VERTEXFORMAT_FLOAT2;
    pipeline_desc.layout.attrs[ATTR_UV_FLIPPED].buffer_index = ATTR_UV_FLIPPED;

    pipeline_desc.layout.attrs[ATTR_HEIGHT].format = SG_VERTEXFORMAT_FLOAT;
    pipeline_desc.layout.attrs[ATTR_HEIGHT].buffer_index = ATTR_HEIGHT;

    //pipeline_desc.rasterizer.face_winding = SG_FACEWINDING_CCW;
    pipeline_desc.blend.enabled = true;
    pipeline_desc.blend.src_factor_rgb = SG_BLENDFACTOR_SRC_ALPHA;
    pipeline_desc.blend.dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
    pipeline_desc.blend.src_factor_alpha = SG_BLENDFACTOR_SRC_ALPHA;
    pipeline_desc.blend.dst_factor_alpha = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
    pipeline_desc.blend.depth_format = SG_PIXELFORMAT_NONE;

    sg_pipeline pipeline_handle = sg_make_pipeline(pipeline_desc);
    const sg_resource_state pipeline_state = sg_query_pipeline_state(pipeline_handle);
    if(pipeline_state != SG_RESOURCESTATE_VALID)
        System::Log("Failed to create sprite pipeline.");

    return std::make_unique<PipelineImpl>(pipeline_handle, shader_handle);
}

void SpritePipeline::Apply(
    IPipeline* pipeline,
    const IRenderBuffer* position,
    const IRenderBuffer* offsets,
    const IRenderBuffer* uv_coordinates,
    const IRenderBuffer* uv_coordinates_flipped,
    const IRenderBuffer* heights,
    const IElementBuffer* indices,
    const ITexture* texture,
    uint32_t buffer_offset)
{
    pipeline->Apply();

    sg_bindings bindings = {};
    bindings.vertex_buffers[ATTR_POSITION].id = position->Id();
    bindings.vertex_buffers[ATTR_POSITION_OFFSET].id = offsets->Id();
    bindings.vertex_buffers[ATTR_UV].id = uv_coordinates->Id();
    bindings.vertex_buffers[ATTR_UV_FLIPPED].id = uv_coordinates_flipped->Id();
    bindings.vertex_buffers[ATTR_HEIGHT].id = heights->Id();

    bindings.vertex_buffer_offsets[ATTR_POSITION] = position->ByteOffsetToIndex(buffer_offset);
    bindings.vertex_buffer_offsets[ATTR_POSITION_OFFSET] = offsets->ByteOffsetToIndex(buffer_offset);
    bindings.vertex_buffer_offsets[ATTR_UV] = uv_coordinates->ByteOffsetToIndex(buffer_offset);
    bindings.vertex_buffer_offsets[ATTR_UV_FLIPPED] = uv_coordinates_flipped->ByteOffsetToIndex(buffer_offset);
    bindings.vertex_buffer_offsets[ATTR_HEIGHT] = heights->ByteOffsetToIndex(buffer_offset);

    bindings.fs_images[0].id = texture->Id();
    bindings.index_buffer.id = indices->Id();

    sg_apply_bindings(&bindings);
}

void SpritePipeline::SetTime(float total_time_s, float delta_time_s)
{
    struct TimeBlock
    {
        float total_time;
        float delta_time;
    } time_block;

    time_block.total_time = total_time_s;
    time_block.delta_time = delta_time_s;

    sg_apply_uniforms(SG_SHADERSTAGE_VS, U_VS_TIME_BLOCK, &time_block, sizeof(TimeBlock));
}

void SpritePipeline::SetTransforms(const math::Matrix& projection, const math::Matrix& view, const math::Matrix& model)
{
    struct TransformBlock
    {
        math::Matrix projection;
        math::Matrix view;
        math::Matrix model;
    } transform_block;

    transform_block.projection = projection;
    transform_block.view = view;
    transform_block.model = model;

    sg_apply_uniforms(SG_SHADERSTAGE_VS, U_VS_TRANSFORM_BLOCK, &transform_block, sizeof(TransformBlock));
}

void SpritePipeline::SetFlipSprite(bool flip_horizontal, bool flip_vertical)
{
    struct FlipInputBlock
    {
        float flip_horizontal;
        float flip_vertical;
    } flip_sprite_block;

    flip_sprite_block.flip_horizontal = flip_horizontal ? 1.0f : 0.0f;
    flip_sprite_block.flip_vertical = flip_vertical ? 1.0f : 0.0f;

    sg_apply_uniforms(SG_SHADERSTAGE_VS, U_VS_FLIP_SPRITE_BLOCK, &flip_sprite_block, sizeof(FlipInputBlock));
}

void SpritePipeline::SetWindSway(bool enable_wind)
{
    const float value = enable_wind ? 1.0f : 0.0f;
    sg_apply_uniforms(SG_SHADERSTAGE_VS, U_VS_WIND_SWAY_BLOCK, &value, sizeof(float));
}

void SpritePipeline::SetShade(const mono::Color::RGBA& color)
{
    sg_apply_uniforms(SG_SHADERSTAGE_FS, U_FS_COLOR_SHADE_BLOCK, &color, sizeof(mono::Color::RGBA));
}

void SpritePipeline::SetFlashSprite(bool flash)
{
    const float value = flash ? 1.0f : 0.0f;
    sg_apply_uniforms(SG_SHADERSTAGE_FS, U_FS_FLASH_BLOCK, &value, sizeof(float));
}

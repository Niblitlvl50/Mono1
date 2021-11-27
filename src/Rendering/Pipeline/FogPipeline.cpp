
#include "FogPipeline.h"
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

        uniform TimeInput time_input;
        uniform TransformInput transform_input;

        layout (location = 0) in vec2 vertex_position;

        out vec2 v_texture_coord;
        out float v_total_time;

        void main()
        {
            const vec2 madd = vec2(0.5, 0.5);
            gl_Position = 
                transform_input.projection *
                transform_input.view *
                transform_input.model *
                vec4(vertex_position, 0.0, 1.0);
            v_texture_coord = vertex_position * madd + madd;
            v_total_time = time_input.total_time;
        }
    )";

    constexpr const char* fragment_source = R"(
        #version 330

        uniform sampler2D sampler;
        uniform vec4 color_shade;

        in vec2 v_texture_coord;
        in float v_total_time;
        out vec4 frag_color;

        void main()
        {
            float time = v_total_time * 0.05;
            vec4 sampled_color = texture(sampler, v_texture_coord + time) - texture(sampler, v_texture_coord - time);
            frag_color = vec4(sampled_color.rgb, 0.25f) * color_shade;
        }
    )";


    constexpr int U_TIME_BLOCK = 0;
    constexpr int U_TRANSFORM_BLOCK = 1;
    constexpr int U_COLOR_SHADE_BLOCK = 0;

    constexpr int ATTR_POSITION = 0;
}

using namespace mono;

mono::IPipelinePtr FogPipeline::MakePipeline()
{
    sg_shader_desc shader_desc = {};
    shader_desc.vs.source = vertex_source;
    shader_desc.attrs[ATTR_POSITION].name = "vertex_position";

    shader_desc.vs.uniform_blocks[U_TIME_BLOCK].size = sizeof(float) * 2;
    shader_desc.vs.uniform_blocks[U_TIME_BLOCK].uniforms[0].name = "time_input.total_time";
    shader_desc.vs.uniform_blocks[U_TIME_BLOCK].uniforms[0].type = SG_UNIFORMTYPE_FLOAT;
    shader_desc.vs.uniform_blocks[U_TIME_BLOCK].uniforms[1].name = "time_input.delta_time";
    shader_desc.vs.uniform_blocks[U_TIME_BLOCK].uniforms[1].type = SG_UNIFORMTYPE_FLOAT;

    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].size = sizeof(math::Matrix) * 3;
    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].uniforms[0].name = "transform_input.projection";
    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].uniforms[0].type = SG_UNIFORMTYPE_MAT4;
    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].uniforms[1].name = "transform_input.view";
    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].uniforms[1].type = SG_UNIFORMTYPE_MAT4;
    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].uniforms[2].name = "transform_input.model";
    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].uniforms[2].type = SG_UNIFORMTYPE_MAT4;

    shader_desc.fs.source = fragment_source;

    shader_desc.fs.images[0].name = "sampler";
    shader_desc.fs.images[0].image_type = SG_IMAGETYPE_2D;
    shader_desc.fs.images[0].sampler_type = SG_SAMPLERTYPE_FLOAT;

    shader_desc.fs.uniform_blocks[U_COLOR_SHADE_BLOCK].size = sizeof(mono::Color::RGBA);
    shader_desc.fs.uniform_blocks[U_COLOR_SHADE_BLOCK].uniforms[0].name = "color_shade";
    shader_desc.fs.uniform_blocks[U_COLOR_SHADE_BLOCK].uniforms[0].type = SG_UNIFORMTYPE_FLOAT4;

    sg_shader shader_handle = sg_make_shader(&shader_desc);

    const sg_resource_state shader_state = sg_query_shader_state(shader_handle);
    if(shader_state != SG_RESOURCESTATE_VALID)
        System::Log("Failed to create texture shader.");

    sg_pipeline_desc pipeline_desc = {};
    pipeline_desc.primitive_type = SG_PRIMITIVETYPE_TRIANGLES;
    pipeline_desc.index_type = SG_INDEXTYPE_UINT16;
    pipeline_desc.shader = shader_handle;

    pipeline_desc.layout.attrs[ATTR_POSITION].format = SG_VERTEXFORMAT_FLOAT2;
    pipeline_desc.layout.attrs[ATTR_POSITION].buffer_index = ATTR_POSITION;

    //pipeline_desc.rasterizer.face_winding = SG_FACEWINDING_CCW;
    pipeline_desc.colors[0].blend.enabled = true;
    pipeline_desc.colors[0].blend.src_factor_rgb = SG_BLENDFACTOR_SRC_ALPHA;
    pipeline_desc.colors[0].blend.dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;

    pipeline_desc.depth.pixel_format = SG_PIXELFORMAT_NONE;

    sg_pipeline pipeline_handle = sg_make_pipeline(pipeline_desc);
    const sg_resource_state pipeline_state = sg_query_pipeline_state(pipeline_handle);
    if(pipeline_state != SG_RESOURCESTATE_VALID)
        System::Log("Failed to create texture pipeline.");

    return std::make_unique<PipelineImpl>(pipeline_handle, shader_handle);
}

void FogPipeline::Apply(
    IPipeline* pipeline,
    const IRenderBuffer* position,
    const IElementBuffer* indices,
    const ITexture* texture)
{
    pipeline->Apply();

    sg_bindings bindings = {};
    bindings.vertex_buffers[ATTR_POSITION].id = position->Id();

    bindings.index_buffer.id = indices->Id();
    bindings.fs_images[0].id = texture->Id();

    sg_apply_bindings(&bindings);
}

void FogPipeline::SetTime(float total_time_s, float delta_time_s)
{
    struct TimeBlock
    {
        float total_time;
        float delta_time;
    } time_block;

    time_block.total_time = total_time_s;
    time_block.delta_time = delta_time_s;

    sg_apply_uniforms(SG_SHADERSTAGE_VS, U_TIME_BLOCK, { &time_block, sizeof(TimeBlock) });
}

void FogPipeline::SetTransforms(const math::Matrix& projection, const math::Matrix& view, const math::Matrix& model)
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

    sg_apply_uniforms(SG_SHADERSTAGE_VS, U_TRANSFORM_BLOCK, { &transform_block, sizeof(TransformBlock) });
}

void FogPipeline::SetShade(const mono::Color::RGBA& color)
{
    sg_apply_uniforms(SG_SHADERSTAGE_FS, U_COLOR_SHADE_BLOCK, { &color, sizeof(mono::Color::RGBA) });
}


#include "WaterPipeline.h"
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

        struct TransformInput
        {
            mat4 projection;
            mat4 view;
            mat4 model;
        };

        uniform TransformInput transform_input;

        layout (location = 0) in vec2 vertex_position;
        layout (location = 1) in vec2 texture_coord;

        out vec2 v_texture_coord;

        void main()
        {
            gl_Position =
                transform_input.projection *
                transform_input.view *
                transform_input.model *
                vec4(vertex_position, 0.0, 1.0);
            v_texture_coord = texture_coord;
        }
    )";

    constexpr const char* fragment_source = R"(
        #version 330

        uniform sampler2D sampler;
        uniform float total_time;
        uniform vec4 color_shade;

        in vec2 v_texture_coord;
        out vec4 frag_color;

        void main()
        {
            vec2 uv = v_texture_coord;
            uv.x += sin(uv.y * 10.0 + total_time * 1.2) * 0.015;
            uv.y += sin(uv.x * 10.0 + total_time * 0.9) * 0.015;

            frag_color = texture(sampler, uv) * color_shade;
        }
    )";

    constexpr const char* vertex_source_annotation = R"(
        #version 330

        struct TransformInput
        {
            mat4 projection;
            mat4 view;
            mat4 model;
        };

        uniform TransformInput transform_input;

        layout (location = 0) in vec2 vertex_position;
        layout (location = 1) in vec4 annotation;

        out vec4 v_annotation;

        void main()
        {
            gl_Position =
                transform_input.projection *
                transform_input.view *
                transform_input.model *
                vec4(vertex_position, 0.0, 1.0);
            v_annotation = annotation;
        }
    )";

    constexpr const char* fragment_source_annotation = R"(
        #version 330

        uniform sampler2D sampler;
        uniform float total_time;
        uniform vec4 color_shade;
        uniform float edge_fade;

        in vec4 v_annotation;
        out vec4 frag_color;

        void main()
        {
            vec2 uv = v_annotation.yx;
            uv.x += sin(uv.y * 10.0 + total_time * 1.2) * 0.015;
            uv.y += sin(uv.x * 10.0 + total_time * 0.9) * 0.015;

            vec4 sampled_color = texture(sampler, uv) * color_shade;

            float v_norm = v_annotation.z;
            float fade_end = 1.0 - edge_fade;
            if(v_norm < edge_fade)
                sampled_color.a *= smoothstep(0.0, edge_fade, v_norm);
            else if(v_norm > fade_end)
                sampled_color.a *= 1.0 - smoothstep(fade_end, 1.0, v_norm);

            frag_color = sampled_color;
        }
    )";

    constexpr int U_TRANSFORM_BLOCK = 0;
    constexpr int U_TIME_BLOCK      = 0;
    constexpr int U_COLOR_BLOCK     = 1;

    constexpr int ATTR_POSITION    = 0;
    constexpr int ATTR_UV          = 1;
    constexpr int ATTR_ANNOTATION  = 1;

    constexpr int U_EDGE_FADE_BLOCK = 2;
}

using namespace mono;

mono::IPipelinePtr WaterPipeline::MakePipeline()
{
    sg_shader_desc shader_desc = {};
    shader_desc.vs.source = vertex_source;
    shader_desc.attrs[ATTR_POSITION].name = "vertex_position";
    shader_desc.attrs[ATTR_UV].name = "texture_coord";

    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].size = sizeof(math::Matrix) * 3;
    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].uniforms[0].name = "transform_input.projection";
    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].uniforms[0].type = SG_UNIFORMTYPE_MAT4;
    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].uniforms[1].name = "transform_input.view";
    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].uniforms[1].type = SG_UNIFORMTYPE_MAT4;
    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].uniforms[2].name = "transform_input.model";
    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].uniforms[2].type = SG_UNIFORMTYPE_MAT4;

    shader_desc.fs.source = fragment_source;

    shader_desc.fs.images[0].used = true;
    shader_desc.fs.images[0].image_type = SG_IMAGETYPE_2D;
    shader_desc.fs.images[0].sample_type = SG_IMAGESAMPLETYPE_FLOAT;
    shader_desc.fs.samplers[0].used = true;
    shader_desc.fs.samplers[0].sampler_type = SG_SAMPLERTYPE_SAMPLE;
    shader_desc.fs.image_sampler_pairs[0] = { true, 0, 0, "sampler" };

    shader_desc.fs.uniform_blocks[U_TIME_BLOCK].size = sizeof(float);
    shader_desc.fs.uniform_blocks[U_TIME_BLOCK].uniforms[0].name = "total_time";
    shader_desc.fs.uniform_blocks[U_TIME_BLOCK].uniforms[0].type = SG_UNIFORMTYPE_FLOAT;

    shader_desc.fs.uniform_blocks[U_COLOR_BLOCK].size = sizeof(mono::Color::RGBA);
    shader_desc.fs.uniform_blocks[U_COLOR_BLOCK].uniforms[0].name = "color_shade";
    shader_desc.fs.uniform_blocks[U_COLOR_BLOCK].uniforms[0].type = SG_UNIFORMTYPE_FLOAT4;

    sg_shader shader_handle = sg_make_shader(&shader_desc);

    const sg_resource_state shader_state = sg_query_shader_state(shader_handle);
    if(shader_state != SG_RESOURCESTATE_VALID)
        System::Log("Failed to create water shader.");

    sg_pipeline_desc pipeline_desc = {};
    pipeline_desc.primitive_type = SG_PRIMITIVETYPE_TRIANGLES;
    pipeline_desc.index_type = SG_INDEXTYPE_UINT16;
    pipeline_desc.shader = shader_handle;

    pipeline_desc.layout.attrs[ATTR_POSITION].format = SG_VERTEXFORMAT_FLOAT2;
    pipeline_desc.layout.attrs[ATTR_POSITION].buffer_index = ATTR_POSITION;

    pipeline_desc.layout.attrs[ATTR_UV].format = SG_VERTEXFORMAT_FLOAT2;
    pipeline_desc.layout.attrs[ATTR_UV].buffer_index = ATTR_UV;

    pipeline_desc.colors[0].blend.enabled = true;
    pipeline_desc.colors[0].blend.src_factor_rgb = SG_BLENDFACTOR_SRC_ALPHA;
    pipeline_desc.colors[0].blend.dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;

    sg_pipeline pipeline_handle = sg_make_pipeline(pipeline_desc);
    const sg_resource_state pipeline_state = sg_query_pipeline_state(pipeline_handle);
    if(pipeline_state != SG_RESOURCESTATE_VALID)
        System::Log("Failed to create water pipeline.");

    return std::make_unique<PipelineImpl>(pipeline_handle, shader_handle);
}

void WaterPipeline::Apply(
    IPipeline* pipeline,
    const IRenderBuffer* position,
    const IRenderBuffer* uv_coordinates,
    const IElementBuffer* indices,
    const ITexture* texture)
{
    pipeline->Apply();

    sg_bindings bindings = {};
    bindings.vertex_buffers[ATTR_POSITION].id = position->Id();
    bindings.vertex_buffers[ATTR_UV].id = uv_coordinates->Id();
    bindings.index_buffer.id = indices->Id();
    bindings.fs.images[0].id = texture->Id();
    bindings.fs.samplers[0].id = texture->SamplerId();

    sg_apply_bindings(&bindings);
}

void WaterPipeline::SetTime(float total_time_s)
{
    sg_apply_uniforms(SG_SHADERSTAGE_FS, U_TIME_BLOCK, { &total_time_s, sizeof(float) });
}

void WaterPipeline::SetTransforms(const math::Matrix& projection, const math::Matrix& view, const math::Matrix& model)
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

void WaterPipeline::SetShade(const mono::Color::RGBA& color)
{
    sg_apply_uniforms(SG_SHADERSTAGE_FS, U_COLOR_BLOCK, { &color, sizeof(mono::Color::RGBA) });
}

mono::IPipelinePtr WaterPipeline::MakeAnnotationPipeline()
{
    sg_shader_desc shader_desc = {};
    shader_desc.vs.source = vertex_source_annotation;
    shader_desc.attrs[ATTR_POSITION].name = "vertex_position";
    shader_desc.attrs[ATTR_ANNOTATION].name = "annotation";

    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].size = sizeof(math::Matrix) * 3;
    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].uniforms[0].name = "transform_input.projection";
    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].uniforms[0].type = SG_UNIFORMTYPE_MAT4;
    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].uniforms[1].name = "transform_input.view";
    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].uniforms[1].type = SG_UNIFORMTYPE_MAT4;
    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].uniforms[2].name = "transform_input.model";
    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].uniforms[2].type = SG_UNIFORMTYPE_MAT4;

    shader_desc.fs.source = fragment_source_annotation;

    shader_desc.fs.images[0].used = true;
    shader_desc.fs.images[0].image_type = SG_IMAGETYPE_2D;
    shader_desc.fs.images[0].sample_type = SG_IMAGESAMPLETYPE_FLOAT;
    shader_desc.fs.samplers[0].used = true;
    shader_desc.fs.samplers[0].sampler_type = SG_SAMPLERTYPE_SAMPLE;
    shader_desc.fs.image_sampler_pairs[0] = { true, 0, 0, "sampler" };

    shader_desc.fs.uniform_blocks[U_TIME_BLOCK].size = sizeof(float);
    shader_desc.fs.uniform_blocks[U_TIME_BLOCK].uniforms[0].name = "total_time";
    shader_desc.fs.uniform_blocks[U_TIME_BLOCK].uniforms[0].type = SG_UNIFORMTYPE_FLOAT;

    shader_desc.fs.uniform_blocks[U_COLOR_BLOCK].size = sizeof(mono::Color::RGBA);
    shader_desc.fs.uniform_blocks[U_COLOR_BLOCK].uniforms[0].name = "color_shade";
    shader_desc.fs.uniform_blocks[U_COLOR_BLOCK].uniforms[0].type = SG_UNIFORMTYPE_FLOAT4;

    shader_desc.fs.uniform_blocks[U_EDGE_FADE_BLOCK].size = sizeof(float);
    shader_desc.fs.uniform_blocks[U_EDGE_FADE_BLOCK].uniforms[0].name = "edge_fade";
    shader_desc.fs.uniform_blocks[U_EDGE_FADE_BLOCK].uniforms[0].type = SG_UNIFORMTYPE_FLOAT;

    sg_shader shader_handle = sg_make_shader(&shader_desc);

    const sg_resource_state shader_state = sg_query_shader_state(shader_handle);
    if(shader_state != SG_RESOURCESTATE_VALID)
        System::Log("Failed to create water annotation shader.");

    sg_pipeline_desc pipeline_desc = {};
    pipeline_desc.primitive_type = SG_PRIMITIVETYPE_TRIANGLES;
    pipeline_desc.index_type = SG_INDEXTYPE_UINT16;
    pipeline_desc.shader = shader_handle;

    pipeline_desc.layout.attrs[ATTR_POSITION].format = SG_VERTEXFORMAT_FLOAT2;
    pipeline_desc.layout.attrs[ATTR_POSITION].buffer_index = ATTR_POSITION;

    pipeline_desc.layout.attrs[ATTR_ANNOTATION].format = SG_VERTEXFORMAT_FLOAT4;
    pipeline_desc.layout.attrs[ATTR_ANNOTATION].buffer_index = ATTR_ANNOTATION;

    pipeline_desc.colors[0].blend.enabled = true;
    pipeline_desc.colors[0].blend.src_factor_rgb = SG_BLENDFACTOR_SRC_ALPHA;
    pipeline_desc.colors[0].blend.dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;

    sg_pipeline pipeline_handle = sg_make_pipeline(pipeline_desc);
    const sg_resource_state pipeline_state = sg_query_pipeline_state(pipeline_handle);
    if(pipeline_state != SG_RESOURCESTATE_VALID)
        System::Log("Failed to create water annotation pipeline.");

    return std::make_unique<PipelineImpl>(pipeline_handle, shader_handle);
}

void WaterPipeline::ApplyAnnotation(
    IPipeline* pipeline,
    const IRenderBuffer* position,
    const IRenderBuffer* annotations,
    const IElementBuffer* indices,
    const ITexture* texture)
{
    pipeline->Apply();

    sg_bindings bindings = {};
    bindings.vertex_buffers[ATTR_POSITION].id = position->Id();
    bindings.vertex_buffers[ATTR_ANNOTATION].id = annotations->Id();
    bindings.index_buffer.id = indices->Id();
    bindings.fs.images[0].id = texture->Id();
    bindings.fs.samplers[0].id = texture->SamplerId();

    sg_apply_bindings(&bindings);
}

void WaterPipeline::SetEdgeFade(float edge_fade)
{
    sg_apply_uniforms(SG_SHADERSTAGE_FS, U_EDGE_FADE_BLOCK, { &edge_fade, sizeof(float) });
}


#include "TexturePipeline.h"
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

        //struct TimeInput
        //{
        //    float total_time;
        //    float delta_time;
        //};

        struct TransformInput
        {
            mat4 projection;
            mat4 view;
            mat4 model;
        };

        //uniform TimeInput time_input;
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
        uniform float is_alpha_texture;
        uniform float enable_blur;
        uniform vec4 color_shade;

        in vec2 v_texture_coord;
        out vec4 frag_color;

        void main()
        {
            vec4 sampled_color;
            if(enable_blur == 0.0f)
            {
                sampled_color = texture(sampler, v_texture_coord);
            }
            else
            {
                vec2 size = 1.0 / textureSize(sampler, 0);

                vec4 original_sample = texture(sampler, v_texture_coord);
                vec4 sample1 = texture(sampler, v_texture_coord + vec2(size.x, 0.0));
                vec4 sample2 = texture(sampler, v_texture_coord + vec2(-size.x, 0.0));
                vec4 sample3 = texture(sampler, v_texture_coord + vec2(0.0, size.y));
                vec4 sample4 = texture(sampler, v_texture_coord + vec2(0.0, -size.y));

                vec4 sample5 = texture(sampler, v_texture_coord + size);
                vec4 sample6 = texture(sampler, v_texture_coord + -size);
                vec4 sample7 = texture(sampler, v_texture_coord + vec2(size.x, -size.y));
                vec4 sample8 = texture(sampler, v_texture_coord + vec2(-size.x, size.y));

                sampled_color = original_sample + (0.125 * (sample1 + sample2 + sample3 + sample4 + sample5 + sample6 + sample7 + sample8));
            }

            if(is_alpha_texture != 0.0)
                sampled_color = vec4(sampled_color.r);

            frag_color = sampled_color * color_shade;
        }
    )";

    //constexpr int U_TIME_BLOCK = 0;
    constexpr int U_TRANSFORM_BLOCK = 0;

    constexpr int U_IS_ALPHA_BLOCK = 0;
    constexpr int U_ENABLE_BLUR_BLOCK = 1;
    constexpr int U_COLOR_SHADE_BLOCK = 2;

    constexpr int ATTR_POSITION = 0;
    constexpr int ATTR_UV = 1;
    constexpr int ATTR_ANNOTATION = 1;



    constexpr const char* vertex_source_annotation = R"(
        #version 330

        //struct TimeInput
        //{
        //    float total_time;
        //    float delta_time;
        //};

        struct TransformInput
        {
            mat4 projection;
            mat4 view;
            mat4 model;
        };

        //uniform TimeInput time_input;
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
        uniform float is_alpha_texture;
        uniform float enable_blur;
        uniform vec4 color_shade;

        in vec4 v_annotation;
        out vec4 frag_color;

        void main()
        {
            vec2 uv = v_annotation.yx;
                
            vec4 sampled_color = texture(sampler, uv);
            if(enable_blur != 0.0f)
                sampled_color = vec4(1, 0, 0, 1);

            if(uv.x < 0.1)
                sampled_color.a *= smoothstep(0.0, 0.1, uv.x);
            else if(uv.x > 0.9)
                sampled_color.a *= 1.0 - smoothstep(0.9, 1.0, uv.x);

            if(is_alpha_texture != 0.0)
                sampled_color = vec4(sampled_color.r);

            frag_color = sampled_color * color_shade;
        }
    )";

}

using namespace mono;

mono::IPipelinePtr TexturePipeline::MakePipeline()
{
    sg_shader_desc shader_desc = {};
    shader_desc.vs.source = vertex_source;
    shader_desc.attrs[ATTR_POSITION].name = "vertex_position";
    shader_desc.attrs[ATTR_UV].name = "texture_coord";

    /*
    shader_desc.vs.uniform_blocks[0].size = sizeof(float) * 2;
    shader_desc.vs.uniform_blocks[0].uniforms[0].name = "time_input.total_time";
    shader_desc.vs.uniform_blocks[0].uniforms[0].type = SG_UNIFORMTYPE_FLOAT;
    shader_desc.vs.uniform_blocks[0].uniforms[1].name = "time_input.delta_time";
    shader_desc.vs.uniform_blocks[0].uniforms[1].type = SG_UNIFORMTYPE_FLOAT;
    */

    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].size = sizeof(math::Matrix) * 3;
    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].uniforms[0].name = "transform_input.projection";
    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].uniforms[0].type = SG_UNIFORMTYPE_MAT4;
    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].uniforms[1].name = "transform_input.view";
    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].uniforms[1].type = SG_UNIFORMTYPE_MAT4;
    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].uniforms[2].name = "transform_input.model";
    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].uniforms[2].type = SG_UNIFORMTYPE_MAT4;

    shader_desc.fs.source = fragment_source;

    shader_desc.fs.images[0].name = "sampler";
    shader_desc.fs.images[0].type = SG_IMAGETYPE_2D;
    shader_desc.fs.images[0].sampler_type = SG_SAMPLERTYPE_FLOAT;

    shader_desc.fs.uniform_blocks[U_IS_ALPHA_BLOCK].size = sizeof(float);
    shader_desc.fs.uniform_blocks[U_IS_ALPHA_BLOCK].uniforms[0].name = "is_alpha_texture";
    shader_desc.fs.uniform_blocks[U_IS_ALPHA_BLOCK].uniforms[0].type = SG_UNIFORMTYPE_FLOAT;

    shader_desc.fs.uniform_blocks[U_ENABLE_BLUR_BLOCK].size = sizeof(float);
    shader_desc.fs.uniform_blocks[U_ENABLE_BLUR_BLOCK].uniforms[0].name = "enable_blur";
    shader_desc.fs.uniform_blocks[U_ENABLE_BLUR_BLOCK].uniforms[0].type = SG_UNIFORMTYPE_FLOAT;

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

    pipeline_desc.layout.attrs[ATTR_UV].format = SG_VERTEXFORMAT_FLOAT2;
    pipeline_desc.layout.attrs[ATTR_UV].buffer_index = ATTR_UV;

    //pipeline_desc.rasterizer.face_winding = SG_FACEWINDING_CCW;
    pipeline_desc.blend.enabled = true;
    pipeline_desc.blend.src_factor_rgb = SG_BLENDFACTOR_SRC_ALPHA;
    pipeline_desc.blend.dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
    pipeline_desc.blend.depth_format = SG_PIXELFORMAT_NONE;

    sg_pipeline pipeline_handle = sg_make_pipeline(pipeline_desc);
    const sg_resource_state pipeline_state = sg_query_pipeline_state(pipeline_handle);
    if(pipeline_state != SG_RESOURCESTATE_VALID)
        System::Log("Failed to create texture pipeline.");

    return std::make_unique<PipelineImpl>(pipeline_handle, shader_handle);
}

mono::IPipelinePtr TexturePipeline::MakeAnnotationPipeline()
{
    sg_shader_desc shader_desc = {};
    shader_desc.vs.source = vertex_source_annotation;
    shader_desc.attrs[ATTR_POSITION].name = "vertex_position";
    shader_desc.attrs[ATTR_ANNOTATION].name = "annotation";

    /*
    shader_desc.vs.uniform_blocks[0].size = sizeof(float) * 2;
    shader_desc.vs.uniform_blocks[0].uniforms[0].name = "time_input.total_time";
    shader_desc.vs.uniform_blocks[0].uniforms[0].type = SG_UNIFORMTYPE_FLOAT;
    shader_desc.vs.uniform_blocks[0].uniforms[1].name = "time_input.delta_time";
    shader_desc.vs.uniform_blocks[0].uniforms[1].type = SG_UNIFORMTYPE_FLOAT;
    */

    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].size = sizeof(math::Matrix) * 3;
    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].uniforms[0].name = "transform_input.projection";
    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].uniforms[0].type = SG_UNIFORMTYPE_MAT4;
    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].uniforms[1].name = "transform_input.view";
    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].uniforms[1].type = SG_UNIFORMTYPE_MAT4;
    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].uniforms[2].name = "transform_input.model";
    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].uniforms[2].type = SG_UNIFORMTYPE_MAT4;

    shader_desc.fs.source = fragment_source_annotation;

    shader_desc.fs.images[0].name = "sampler";
    shader_desc.fs.images[0].type = SG_IMAGETYPE_2D;
    shader_desc.fs.images[0].sampler_type = SG_SAMPLERTYPE_FLOAT;

    shader_desc.fs.uniform_blocks[U_IS_ALPHA_BLOCK].size = sizeof(float);
    shader_desc.fs.uniform_blocks[U_IS_ALPHA_BLOCK].uniforms[0].name = "is_alpha_texture";
    shader_desc.fs.uniform_blocks[U_IS_ALPHA_BLOCK].uniforms[0].type = SG_UNIFORMTYPE_FLOAT;

    shader_desc.fs.uniform_blocks[U_ENABLE_BLUR_BLOCK].size = sizeof(float);
    shader_desc.fs.uniform_blocks[U_ENABLE_BLUR_BLOCK].uniforms[0].name = "enable_blur";
    shader_desc.fs.uniform_blocks[U_ENABLE_BLUR_BLOCK].uniforms[0].type = SG_UNIFORMTYPE_FLOAT;

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

    pipeline_desc.layout.attrs[ATTR_ANNOTATION].format = SG_VERTEXFORMAT_FLOAT4;
    pipeline_desc.layout.attrs[ATTR_ANNOTATION].buffer_index = ATTR_ANNOTATION;

    //pipeline_desc.rasterizer.face_winding = SG_FACEWINDING_CCW;
    pipeline_desc.blend.enabled = true;
    pipeline_desc.blend.src_factor_rgb = SG_BLENDFACTOR_SRC_ALPHA;
    pipeline_desc.blend.dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
    pipeline_desc.blend.depth_format = SG_PIXELFORMAT_NONE;

    sg_pipeline pipeline_handle = sg_make_pipeline(pipeline_desc);
    const sg_resource_state pipeline_state = sg_query_pipeline_state(pipeline_handle);
    if(pipeline_state != SG_RESOURCESTATE_VALID)
        System::Log("Failed to create texture pipeline.");

    return std::make_unique<PipelineImpl>(pipeline_handle, shader_handle);
}


void TexturePipeline::Apply(
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
    bindings.fs_images[0].id = texture->Id();

    sg_apply_bindings(&bindings);
}

/*
void TexturePipeline::SetTime(IPipeline* pipeline, float total_time_s, float delta_time_s)
{
    struct TimeBlock
    {
        float total_time;
        float delta_time;
    } time_block;

    time_block.total_time = total_time_s;
    time_block.delta_time = delta_time_s;

    sg_apply_uniforms(SG_SHADERSTAGE_VS, U_TIME_BLOCK, &time_block, sizeof(TimeBlock));
}
*/

void TexturePipeline::SetTransforms(const math::Matrix& projection, const math::Matrix& view, const math::Matrix& model)
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

    sg_apply_uniforms(SG_SHADERSTAGE_VS, U_TRANSFORM_BLOCK, &transform_block, sizeof(TransformBlock));
}

void TexturePipeline::SetIsAlpha(bool is_alpha_texture)
{
    const float magic_value = is_alpha_texture ? 1.0f : 0.0f;
    sg_apply_uniforms(SG_SHADERSTAGE_FS, U_IS_ALPHA_BLOCK, &magic_value, sizeof(float));
}

void TexturePipeline::SetBlur(bool enable_blur)
{
    const float magic_value = enable_blur ? 1.0f : 0.0f;
    sg_apply_uniforms(SG_SHADERSTAGE_FS, U_ENABLE_BLUR_BLOCK, &magic_value, sizeof(float));
}

void TexturePipeline::SetShade(const mono::Color::RGBA& color)
{
    sg_apply_uniforms(SG_SHADERSTAGE_FS, U_COLOR_SHADE_BLOCK, &color, sizeof(mono::Color::RGBA));
}

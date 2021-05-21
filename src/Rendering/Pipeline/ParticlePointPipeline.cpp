
#include "ParticlePointPipeline.h"
#include "Impl/PipelineImpl.h"

#include "Math/Matrix.h"
#include "Rendering/RenderBuffer/IRenderBuffer.h"
#include "Rendering/Texture/ITexture.h"
#include "System/System.h"

#include "sokol/sokol_gfx.h"

namespace
{
    constexpr const char* vertex_source = R"(
        #version 330

        /*
        struct TimeInput
        {
            float total_time;
            float delta_time;
        };
        */

        struct TransformInput
        {
            mat4 projection;
            mat4 view;
            mat4 model;
        };

        //uniform TimeInput time_input;
        uniform TransformInput transform_input;

        layout (location = 0) in vec2 vertex_position;
        layout (location = 1) in float vertex_rotation;
        layout (location = 2) in vec4 vertex_color;
        layout (location = 3) in float point_size;

        out vec4 color;
        out float rotation;

        void main()
        {
            gl_Position = transform_input.projection * transform_input.view * transform_input.model * vec4(vertex_position, 0.0, 1.0);
            gl_PointSize = point_size;
            color = vertex_color;
            rotation = vertex_rotation;
        }
    )";

    constexpr const char* fragment_source = R"(
        #version 330

        uniform sampler2D sampler;

        in vec4 color;
        in float rotation;
        out vec4 frag_color;

        void main()
        {
            vec2 coord = gl_PointCoord;
            float sin_factor = sin(rotation);
            float cos_factor = cos(rotation);
            coord = (coord - 0.5) * mat2(cos_factor, sin_factor, -sin_factor, cos_factor);
            coord += 0.5;

            frag_color = texture(sampler, coord).r * color;
        }
    )";


    constexpr int U_TRANSFORM_BLOCK = 0;

    constexpr int ATTR_POSITION = 0;
    constexpr int ATTR_ROTATION = 1;
    constexpr int ATTR_COLOR = 2;
    constexpr int ATTR_POINT_SIZE = 3;
}

using namespace mono;

mono::IPipelinePtr ParticlePointPipeline::MakePipeline()
{
    sg_shader_desc shader_desc = {};
    shader_desc.vs.source = vertex_source;

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
    
    shader_desc.attrs[ATTR_POSITION].name = "vertex_position";
    shader_desc.attrs[ATTR_ROTATION].name = "vertex_rotation";
    shader_desc.attrs[ATTR_COLOR].name = "vertex_color";
    shader_desc.attrs[ATTR_POINT_SIZE].name = "point_size";

    sg_shader shader_handle = sg_make_shader(&shader_desc);

    const sg_resource_state shader_state = sg_query_shader_state(shader_handle);
    if(shader_state != SG_RESOURCESTATE_VALID)
        System::Log("Failed to create particle shader.");

    sg_pipeline_desc pipeline_desc = {};
    pipeline_desc.primitive_type = SG_PRIMITIVETYPE_POINTS;
    pipeline_desc.shader = shader_handle;

    pipeline_desc.layout.attrs[ATTR_POSITION].format = SG_VERTEXFORMAT_FLOAT2;
    pipeline_desc.layout.attrs[ATTR_POSITION].buffer_index = ATTR_POSITION;

    pipeline_desc.layout.attrs[ATTR_ROTATION].format = SG_VERTEXFORMAT_FLOAT;
    pipeline_desc.layout.attrs[ATTR_ROTATION].buffer_index = ATTR_ROTATION;

    pipeline_desc.layout.attrs[ATTR_COLOR].format = SG_VERTEXFORMAT_FLOAT4;
    pipeline_desc.layout.attrs[ATTR_COLOR].buffer_index = ATTR_COLOR;

    pipeline_desc.layout.attrs[ATTR_POINT_SIZE].format = SG_VERTEXFORMAT_FLOAT;
    pipeline_desc.layout.attrs[ATTR_POINT_SIZE].buffer_index = ATTR_POINT_SIZE;

    //pipeline_desc.rasterizer.face_winding = SG_FACEWINDING_CCW;

    pipeline_desc.blend.enabled = true;
    pipeline_desc.blend.src_factor_rgb = SG_BLENDFACTOR_SRC_ALPHA;
    pipeline_desc.blend.dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
    //pipeline_desc.blend.dst_factor_rgb = SG_BLENDFACTOR_ONE;
    pipeline_desc.blend.depth_format = SG_PIXELFORMAT_NONE;

    sg_pipeline pipeline_handle = sg_make_pipeline(pipeline_desc);
    const sg_resource_state pipeline_state = sg_query_pipeline_state(pipeline_handle);
    if(pipeline_state != SG_RESOURCESTATE_VALID)
        System::Log("Failed to create particle pipeline.");

    return std::make_unique<PipelineImpl>(pipeline_handle, shader_handle);
}

void ParticlePointPipeline::Apply(
    IPipeline* pipeline,
    const IRenderBuffer* position,
    const IRenderBuffer* rotation,
    const IRenderBuffer* color,
    const IRenderBuffer* point_size,
    const ITexture* texture)
{
    pipeline->Apply();

    sg_bindings bindings = {};
    bindings.vertex_buffers[ATTR_POSITION].id = position->Id();
    bindings.vertex_buffers[ATTR_ROTATION].id = rotation->Id();
    bindings.vertex_buffers[ATTR_COLOR].id = color->Id();
    bindings.vertex_buffers[ATTR_POINT_SIZE].id = point_size->Id();

    bindings.fs_images[0].id = texture->Id();

    sg_apply_bindings(&bindings);
}

/*
void ParticlePointPipeline::SetTime(float total_time_s, float delta_time_s)
{
    struct TimeBlock
    {
        float total_time;
        float delta_time;
    } time_block;

    time_block.total_time = total_time_s;
    time_block.delta_time = delta_time_s;

    sg_apply_uniforms(SG_SHADERSTAGE_VS, 1, &time_block, sizeof(TimeBlock));
}
*/

void ParticlePointPipeline::SetTransforms(const math::Matrix& projection, const math::Matrix& view, const math::Matrix& model)
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

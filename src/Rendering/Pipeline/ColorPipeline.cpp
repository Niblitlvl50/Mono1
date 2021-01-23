
#include "ColorPipeline.h"
#include "Impl/PipelineImpl.h"

#include "Math/Matrix.h"
#include "Rendering/RenderBuffer/IRenderBuffer.h"
#include "System/System.h"

#include "sokol/sokol_gfx.h"
#include "glad/glad_gl_33/glad.h"

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
        uniform float point_size;

        layout (location = 0) in vec2 vertex_position;
        layout (location = 1) in vec4 vertex_color;

        out vec4 color;

        void main()
        {
            gl_Position = transform_input.projection * transform_input.view * transform_input.model * vec4(vertex_position, 0.0, 1.0);
            gl_PointSize = point_size;
            color = vertex_color;
        }
    )";

    constexpr const char* fragment_source = R"(
        #version 330

        in vec4 color;
        out vec4 frag_color;

        void main()
        {
            frag_color = color;
        }
    )";

    constexpr int U_TRANSFORM_BLOCK = 0;
    constexpr int U_POINT_SIZE_BLOCK = 1;

    constexpr int ATTR_POSITION = 0;
    constexpr int ATTR_COLOR = 1;

    mono::IPipelinePtr MakePipeline(sg_primitive_type primitive_type, sg_index_type index_type)
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

        shader_desc.vs.uniform_blocks[U_POINT_SIZE_BLOCK].size = sizeof(float);
        shader_desc.vs.uniform_blocks[U_POINT_SIZE_BLOCK].uniforms[0].name = "point_size";
        shader_desc.vs.uniform_blocks[U_POINT_SIZE_BLOCK].uniforms[0].type = SG_UNIFORMTYPE_FLOAT;

        shader_desc.fs.source = fragment_source;
        
        shader_desc.attrs[ATTR_POSITION].name = "vertex_position";
        shader_desc.attrs[ATTR_COLOR].name = "vertex_color";

        sg_shader shader_handle = sg_make_shader(&shader_desc);

        const sg_resource_state shader_state = sg_query_shader_state(shader_handle);
        if(shader_state != SG_RESOURCESTATE_VALID)
            System::Log("Failed to create color shader.\n");

        sg_pipeline_desc pipeline_desc = {};
        pipeline_desc.primitive_type = primitive_type;
        pipeline_desc.index_type = index_type;
        pipeline_desc.shader = shader_handle;

        pipeline_desc.layout.attrs[ATTR_POSITION].format = SG_VERTEXFORMAT_FLOAT2;
        pipeline_desc.layout.attrs[ATTR_POSITION].buffer_index = ATTR_POSITION;

        pipeline_desc.layout.attrs[ATTR_COLOR].format = SG_VERTEXFORMAT_FLOAT4;
        pipeline_desc.layout.attrs[ATTR_COLOR].buffer_index = ATTR_COLOR;

        pipeline_desc.blend.enabled = true;
        pipeline_desc.blend.src_factor_rgb = SG_BLENDFACTOR_SRC_ALPHA;
        pipeline_desc.blend.dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;

        sg_pipeline pipeline_handle = sg_make_pipeline(pipeline_desc);
        const sg_resource_state pipeline_state = sg_query_pipeline_state(pipeline_handle);
        if(pipeline_state != SG_RESOURCESTATE_VALID)
            System::Log("Failed to create pipeline.\n");

        return std::make_unique<mono::PipelineImpl>(pipeline_handle, shader_handle);
    }
}

using namespace mono;

mono::IPipelinePtr ColorPipeline::MakePointsPipeline()
{
    return MakePipeline(SG_PRIMITIVETYPE_POINTS, SG_INDEXTYPE_NONE);
}

mono::IPipelinePtr ColorPipeline::MakeLinesPipeline()
{
    return MakePipeline(SG_PRIMITIVETYPE_LINES, SG_INDEXTYPE_NONE);
}

mono::IPipelinePtr ColorPipeline::MakeLineStripPipeline()
{
    return MakePipeline(SG_PRIMITIVETYPE_LINE_STRIP, SG_INDEXTYPE_NONE);
}

mono::IPipelinePtr ColorPipeline::MakeTrianglesPipeline()
{
    return MakePipeline(SG_PRIMITIVETYPE_TRIANGLES, SG_INDEXTYPE_UINT16);
}

void ColorPipeline::Apply(IPipeline* pipeline, const IRenderBuffer* position, const IRenderBuffer* color)
{
    pipeline->Apply();

    sg_bindings bindings = {};
    bindings.vertex_buffers[ATTR_POSITION].id = position->Id();
    bindings.vertex_buffers[ATTR_COLOR].id = color->Id();
    sg_apply_bindings(&bindings);
}

void ColorPipeline::Apply(
    IPipeline* pipeline, const IRenderBuffer* vertices, const IRenderBuffer* colors, const IElementBuffer* indices)
{
    pipeline->Apply();

    sg_bindings bindings = {};
    bindings.vertex_buffers[ATTR_POSITION].id = vertices->Id();
    bindings.vertex_buffers[ATTR_COLOR].id = colors->Id();
    bindings.index_buffer.id = indices->Id();
    sg_apply_bindings(&bindings);
}

void ColorPipeline::SetTime(IPipeline* pipeline, float total_time_s, float delta_time_s)
{
    /*
    struct TimeBlock
    {
        float total_time;
        float delta_time;
    } time_block;

    time_block.total_time = total_time_s;
    time_block.delta_time = delta_time_s;

    sg_apply_uniforms(SG_SHADERSTAGE_VS, 0, &time_block, sizeof(TimeBlock));
    */
}

void ColorPipeline::SetTransforms(
    IPipeline* pipeline, const math::Matrix& projection, const math::Matrix& view, const math::Matrix& model)
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

void ColorPipeline::SetPointSize(IPipeline* pipeline, float point_size)
{
    sg_apply_uniforms(SG_SHADERSTAGE_VS, U_POINT_SIZE_BLOCK, &point_size, sizeof(float));
}

void ColorPipeline::SetLineWidth(IPipeline* pipeline, float line_width)
{
    glLineWidth(line_width);
}


#include "ScreenPipeline.h"
#include "Impl/PipelineImpl.h"

#include "Rendering/RenderBuffer/IRenderBuffer.h"
#include "Rendering/Texture/ITexture.h"
#include "System/System.h"

#include "sokol/sokol_gfx.h"

namespace
{
    constexpr const char* vertex_source = R"(
        #version 330

        layout (location = 0) in vec2 vertex_position;
        layout (location = 1) in vec2 texture_coord;

        out vec2 v_vertex_coord;
        out vec2 v_texture_coord;

        void main()
        {
            gl_Position = vec4(vertex_position, 0.0, 1.0);
            v_texture_coord = texture_coord;
            v_vertex_coord = vertex_position;
        }
    )";

    constexpr const char* fragment_source = R"(
        #version 330

        uniform sampler2D sampler;
        uniform sampler2D sampler_light;
        uniform int fade_corners;
        uniform int invert_colors;
        uniform int lighting_enabled;
        uniform float fade_alpha;

        in vec2 v_texture_coord;
        in vec2 v_vertex_coord;
        out vec4 frag_color;

        void main()
        {
            vec4 scene_color = texture(sampler, v_texture_coord);

            vec4 lights = vec4(1.0);
            if(lighting_enabled != 0)
                lights = texture(sampler_light, v_texture_coord);

            vec4 color = scene_color * lights;

            if(fade_corners != 0)
            {
                float unit_length = length(vec2(1.0, 1.0));
                float length = length(v_vertex_coord);
                float length_diff = unit_length - length;

                float alpha = 1.0;
                if(length > 1.0)
                {
                    float range = unit_length - 1.0;
                    alpha = length_diff / range;
                    color = mix(vec4(0.0, 0.0, 0.0, 1.0), color, vec4(alpha));
                }
            }

            frag_color = color * fade_alpha;

            if(invert_colors != 0)
            {
                // Invert colors
                frag_color = 1.0 - frag_color;
                // frag_color = vec4(vec3(1.0 - frag_color), 1.0f);
                // frag_color = vec4(vec3(1.0 - texture(sampler, v_texture_coord)), 1.0);
            }
        }
    )";

    constexpr int ATTR_POSITION = 0;
    constexpr int ATTR_UV = 1;

    constexpr int U_FADE_CORNERS_BLOCK = 0;
    constexpr int U_INVERT_COLORS_BLOCK = 1;
    constexpr int U_ENABLE_LIGHTING_BLOCK = 2;
    constexpr int U_FADE_ALPHA_BLOCK = 3;
}

using namespace mono;

mono::IPipelinePtr ScreenPipeline::MakePipeline()
{
    sg_shader_desc shader_desc = {};
    shader_desc.vs.source = vertex_source;
    shader_desc.attrs[ATTR_POSITION].name = "vertex_position";
    shader_desc.attrs[ATTR_UV].name = "texture_coord";

    shader_desc.fs.source = fragment_source;

    shader_desc.fs.images[0].name = "sampler";
    shader_desc.fs.images[0].image_type = SG_IMAGETYPE_2D;
    shader_desc.fs.images[0].sampler_type = SG_SAMPLERTYPE_FLOAT;
    shader_desc.fs.images[1].name = "sampler_light";
    shader_desc.fs.images[1].image_type = SG_IMAGETYPE_2D;
    shader_desc.fs.images[1].sampler_type = SG_SAMPLERTYPE_FLOAT;

    shader_desc.fs.uniform_blocks[U_FADE_CORNERS_BLOCK].size = sizeof(int);
    shader_desc.fs.uniform_blocks[U_FADE_CORNERS_BLOCK].uniforms[0].name = "fade_corners";
    shader_desc.fs.uniform_blocks[U_FADE_CORNERS_BLOCK].uniforms[0].type = SG_UNIFORMTYPE_INT;

    shader_desc.fs.uniform_blocks[U_INVERT_COLORS_BLOCK].size = sizeof(int);
    shader_desc.fs.uniform_blocks[U_INVERT_COLORS_BLOCK].uniforms[0].name = "invert_colors";
    shader_desc.fs.uniform_blocks[U_INVERT_COLORS_BLOCK].uniforms[0].type = SG_UNIFORMTYPE_INT;

    shader_desc.fs.uniform_blocks[U_ENABLE_LIGHTING_BLOCK].size = sizeof(int);
    shader_desc.fs.uniform_blocks[U_ENABLE_LIGHTING_BLOCK].uniforms[0].name = "lighting_enabled";
    shader_desc.fs.uniform_blocks[U_ENABLE_LIGHTING_BLOCK].uniforms[0].type = SG_UNIFORMTYPE_INT;

    shader_desc.fs.uniform_blocks[U_FADE_ALPHA_BLOCK].size = sizeof(float);
    shader_desc.fs.uniform_blocks[U_FADE_ALPHA_BLOCK].uniforms[0].name = "fade_alpha";
    shader_desc.fs.uniform_blocks[U_FADE_ALPHA_BLOCK].uniforms[0].type = SG_UNIFORMTYPE_FLOAT;

    sg_shader shader_handle = sg_make_shader(&shader_desc);

    const sg_resource_state shader_state = sg_query_shader_state(shader_handle);
    if(shader_state != SG_RESOURCESTATE_VALID)
        System::Log("Failed to create screen shader.");

    sg_pipeline_desc pipeline_desc = {};
    pipeline_desc.primitive_type = SG_PRIMITIVETYPE_TRIANGLES;
    pipeline_desc.index_type = SG_INDEXTYPE_UINT16;
    pipeline_desc.shader = shader_handle;

    pipeline_desc.layout.attrs[ATTR_POSITION].format = SG_VERTEXFORMAT_FLOAT2;
    pipeline_desc.layout.attrs[ATTR_POSITION].buffer_index = ATTR_POSITION;

    pipeline_desc.layout.attrs[ATTR_UV].format = SG_VERTEXFORMAT_FLOAT2;
    pipeline_desc.layout.attrs[ATTR_UV].buffer_index = ATTR_UV;

    //pipeline_desc.rasterizer.face_winding = SG_FACEWINDING_CCW;
    sg_pipeline pipeline_handle = sg_make_pipeline(pipeline_desc);
    const sg_resource_state pipeline_state = sg_query_pipeline_state(pipeline_handle);
    if(pipeline_state != SG_RESOURCESTATE_VALID)
        System::Log("Failed to create screen pipeline.");

    return std::make_unique<PipelineImpl>(pipeline_handle, shader_handle);}

void ScreenPipeline::Apply(
    IPipeline* pipeline,
    const IRenderBuffer* position,
    const IRenderBuffer* uv_coordinates,
    const IElementBuffer* indices,
    const ITexture* color_texture,
    const ITexture* light_texture)
{
    pipeline->Apply();

    sg_bindings bindings = {};
    bindings.vertex_buffers[ATTR_POSITION].id = position->Id();
    bindings.vertex_buffers[ATTR_UV].id = uv_coordinates->Id();

    bindings.index_buffer.id = indices->Id();
    bindings.fs_images[0].id = color_texture->Id();
    bindings.fs_images[1].id = light_texture->Id();

    sg_apply_bindings(&bindings);
}

void ScreenPipeline::FadeCorners(bool enable)
{
    const int int_value = enable ? 1 : 0;
    sg_apply_uniforms(SG_SHADERSTAGE_FS, U_FADE_CORNERS_BLOCK, { &int_value, sizeof(int) });
}

void ScreenPipeline::InvertColors(bool enable)
{
    const int int_value = enable ? 1 : 0;
    sg_apply_uniforms(SG_SHADERSTAGE_FS, U_INVERT_COLORS_BLOCK, { &int_value, sizeof(int) });
}

void ScreenPipeline::EnableLighting(bool enable)
{
    const int int_value = enable ? 1 : 0;
    sg_apply_uniforms(SG_SHADERSTAGE_FS, U_ENABLE_LIGHTING_BLOCK, { &int_value, sizeof(int) });
}

void ScreenPipeline::FadeScreenAlpha(float alpha)
{
    sg_apply_uniforms(SG_SHADERSTAGE_FS, U_FADE_ALPHA_BLOCK, { &alpha, sizeof(float) });
}

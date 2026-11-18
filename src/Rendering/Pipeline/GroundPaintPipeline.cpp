
#include "GroundPaintPipeline.h"
#include "Impl/PipelineImpl.h"

#include "Math/Matrix.h"
#include "Rendering/GroundPaint.h"
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

        out vec2 v_world_position;

        void main()
        {
            vec4 world_position = transform_input.model * vec4(vertex_position, 0.0, 1.0);
            gl_Position = transform_input.projection * transform_input.view * world_position;
            v_world_position = world_position.xy;
        }
    )";

    constexpr const char* fragment_source = R"(
        #version 330

        uniform sampler2D splat_sampler;
        uniform sampler2D layer0_sampler;
        uniform sampler2D layer1_sampler;
        uniform sampler2D layer2_sampler;
        uniform sampler2D layer3_sampler;
        uniform sampler2D layer4_sampler;

        // Lower left corner in xy, size in zw.
        uniform vec4 splat_rect;

        // The scales of layer 1 to 4.
        uniform vec4 layer_scales;

        // Base layer scale, edge roughness, pixels per meter and noise size in meters.
        uniform vec4 params;

        // Blend the layers by weight instead of picking one in x, if the base layer has a texture in y.
        uniform vec4 options;

        // If layer 1 to 4 have a texture, a layer without one is see through.
        uniform vec4 layer_enabled;

        in vec2 v_world_position;
        out vec4 frag_color;

        float hash(vec2 p)
        {
            p = fract(p * vec2(123.34, 456.21));
            p += dot(p, p + 45.32);
            return fract(p.x * p.y);
        }

        float value_noise(vec2 p)
        {
            vec2 i = floor(p);
            vec2 f = fract(p);
            f = f * f * (3.0 - 2.0 * f);

            float a = hash(i);
            float b = hash(i + vec2(1.0, 0.0));
            float c = hash(i + vec2(0.0, 1.0));
            float d = hash(i + vec2(1.0, 1.0));
            return mix(mix(a, b, f.x), mix(c, d, f.x), f.y);
        }

        vec4 sample_layer(sampler2D layer_sampler, vec2 pixel_position, float scale)
        {
            // Texture rows go down, the world goes up.
            vec2 size = vec2(textureSize(layer_sampler, 0)) * scale;
            return texture(layer_sampler, vec2(pixel_position.x, -pixel_position.y) / size);
        }

        void main()
        {
            float pixels_per_meter = params.z;

            // Everything is decided per game pixel, so the edges are as crisp as the rest of the art.
            vec2 pixel_position = floor(v_world_position * pixels_per_meter) + 0.5;
            vec2 world_position = pixel_position / pixels_per_meter;

            vec4 weights = texture(splat_sampler, (world_position - splat_rect.xy) / splat_rect.zw);
            float base_weight = 1.0 - clamp(weights.r + weights.g + weights.b + weights.a, 0.0, 1.0);

            vec2 noise_position = world_position / params.w;
            float roughness = params.y;

            float scores[5];
            scores[0] = base_weight + (value_noise(noise_position) - 0.5) * roughness;
            scores[1] = weights.r + (value_noise(noise_position + vec2(17.3, 5.1)) - 0.5) * roughness;
            scores[2] = weights.g + (value_noise(noise_position + vec2(-9.7, 31.4)) - 0.5) * roughness;
            scores[3] = weights.b + (value_noise(noise_position + vec2(43.9, -21.6)) - 0.5) * roughness;
            scores[4] = weights.a + (value_noise(noise_position + vec2(-37.2, -13.8)) - 0.5) * roughness;

            float enabled[5];
            enabled[0] = options.y;
            enabled[1] = layer_enabled.x;
            enabled[2] = layer_enabled.y;
            enabled[3] = layer_enabled.z;
            enabled[4] = layer_enabled.w;

            if(options.x > 0.5)
            {
                // The noise roughens up the blend too, but only where a layer is partly painted, so
                // it doesn't add any of a layer where there is none. The layers with a texture give
                // the color and the ones without make it see through.
                float raw_weights[5];
                raw_weights[0] = base_weight;
                raw_weights[1] = weights.r;
                raw_weights[2] = weights.g;
                raw_weights[3] = weights.b;
                raw_weights[4] = weights.a;

                float blend_weights[5];
                float total_weight = 0.0;
                float visible_weight = 0.0;
                for(int index = 0; index < 5; ++index)
                {
                    float raw_weight = raw_weights[index];
                    float noise = scores[index] - raw_weight;
                    float partly_painted = 4.0 * raw_weight * (1.0 - raw_weight);
                    blend_weights[index] = clamp(raw_weight + noise * partly_painted, 0.0, 1.0);
                    total_weight += blend_weights[index];
                    visible_weight += blend_weights[index] * enabled[index];
                }

                if(total_weight <= 0.0)
                {
                    blend_weights[0] = 1.0;
                    total_weight = 1.0;
                    visible_weight = enabled[0];
                }

                if(visible_weight <= 0.0)
                    discard;

                vec4 color =
                    sample_layer(layer0_sampler, pixel_position, params.x) * blend_weights[0] * enabled[0] +
                    sample_layer(layer1_sampler, pixel_position, layer_scales.x) * blend_weights[1] * enabled[1] +
                    sample_layer(layer2_sampler, pixel_position, layer_scales.y) * blend_weights[2] * enabled[2] +
                    sample_layer(layer3_sampler, pixel_position, layer_scales.z) * blend_weights[3] * enabled[3] +
                    sample_layer(layer4_sampler, pixel_position, layer_scales.w) * blend_weights[4] * enabled[4];

                frag_color = color / visible_weight;
                frag_color.a *= visible_weight / total_weight;
                return;
            }

            int best = 0;
            for(int index = 1; index < 5; ++index)
            {
                if(scores[index] > scores[best])
                    best = index;
            }

            if(enabled[best] < 0.5)
                discard;

            if(best == 0)
                frag_color = sample_layer(layer0_sampler, pixel_position, params.x);
            else if(best == 1)
                frag_color = sample_layer(layer1_sampler, pixel_position, layer_scales.x);
            else if(best == 2)
                frag_color = sample_layer(layer2_sampler, pixel_position, layer_scales.y);
            else if(best == 3)
                frag_color = sample_layer(layer3_sampler, pixel_position, layer_scales.z);
            else
                frag_color = sample_layer(layer4_sampler, pixel_position, layer_scales.w);
        }
    )";

    constexpr int U_TRANSFORM_BLOCK = 0;
    constexpr int U_PARAMS_BLOCK = 0;
    constexpr int ATTR_POSITION = 0;

    // The splat map, then the layers.
    constexpr int N_IMAGES = mono::GROUND_PAINT_LAYERS + 1;
    constexpr const char* sampler_names[N_IMAGES] = {
        "splat_sampler", "layer0_sampler", "layer1_sampler", "layer2_sampler", "layer3_sampler", "layer4_sampler"
    };

    // How big the blobs of the edge noise are.
    constexpr float noise_size_meters = 0.4f;
}

using namespace mono;

mono::IPipelinePtr GroundPaintPipeline::MakePipeline()
{
    sg_shader_desc shader_desc = {};
    shader_desc.vs.source = vertex_source;
    shader_desc.attrs[ATTR_POSITION].name = "vertex_position";

    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].size = sizeof(math::Matrix) * 3;
    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].uniforms[0].name = "transform_input.projection";
    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].uniforms[0].type = SG_UNIFORMTYPE_MAT4;
    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].uniforms[1].name = "transform_input.view";
    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].uniforms[1].type = SG_UNIFORMTYPE_MAT4;
    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].uniforms[2].name = "transform_input.model";
    shader_desc.vs.uniform_blocks[U_TRANSFORM_BLOCK].uniforms[2].type = SG_UNIFORMTYPE_MAT4;

    shader_desc.fs.source = fragment_source;

    for(int index = 0; index < N_IMAGES; ++index)
    {
        shader_desc.fs.images[index].used = true;
        shader_desc.fs.images[index].image_type = SG_IMAGETYPE_2D;
        shader_desc.fs.images[index].sample_type = SG_IMAGESAMPLETYPE_FLOAT;
        shader_desc.fs.samplers[index].used = true;
        shader_desc.fs.samplers[index].sampler_type = SG_SAMPLERTYPE_SAMPLE;
        shader_desc.fs.image_sampler_pairs[index] = { true, index, index, sampler_names[index] };
    }

    shader_desc.fs.uniform_blocks[U_PARAMS_BLOCK].size = sizeof(float) * 4 * 5;
    shader_desc.fs.uniform_blocks[U_PARAMS_BLOCK].uniforms[0].name = "splat_rect";
    shader_desc.fs.uniform_blocks[U_PARAMS_BLOCK].uniforms[0].type = SG_UNIFORMTYPE_FLOAT4;
    shader_desc.fs.uniform_blocks[U_PARAMS_BLOCK].uniforms[1].name = "layer_scales";
    shader_desc.fs.uniform_blocks[U_PARAMS_BLOCK].uniforms[1].type = SG_UNIFORMTYPE_FLOAT4;
    shader_desc.fs.uniform_blocks[U_PARAMS_BLOCK].uniforms[2].name = "params";
    shader_desc.fs.uniform_blocks[U_PARAMS_BLOCK].uniforms[2].type = SG_UNIFORMTYPE_FLOAT4;
    shader_desc.fs.uniform_blocks[U_PARAMS_BLOCK].uniforms[3].name = "options";
    shader_desc.fs.uniform_blocks[U_PARAMS_BLOCK].uniforms[3].type = SG_UNIFORMTYPE_FLOAT4;
    shader_desc.fs.uniform_blocks[U_PARAMS_BLOCK].uniforms[4].name = "layer_enabled";
    shader_desc.fs.uniform_blocks[U_PARAMS_BLOCK].uniforms[4].type = SG_UNIFORMTYPE_FLOAT4;

    sg_shader shader_handle = sg_make_shader(&shader_desc);

    const sg_resource_state shader_state = sg_query_shader_state(shader_handle);
    if(shader_state != SG_RESOURCESTATE_VALID)
        System::Log("Failed to create ground paint shader.");

    sg_pipeline_desc pipeline_desc = {};
    pipeline_desc.primitive_type = SG_PRIMITIVETYPE_TRIANGLES;
    pipeline_desc.index_type = SG_INDEXTYPE_UINT16;
    pipeline_desc.shader = shader_handle;

    pipeline_desc.layout.attrs[ATTR_POSITION].format = SG_VERTEXFORMAT_FLOAT2;
    pipeline_desc.layout.attrs[ATTR_POSITION].buffer_index = ATTR_POSITION;

    pipeline_desc.colors[0].blend.enabled = true;
    pipeline_desc.colors[0].blend.src_factor_rgb = SG_BLENDFACTOR_SRC_ALPHA;
    pipeline_desc.colors[0].blend.dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;

    sg_pipeline pipeline_handle = sg_make_pipeline(pipeline_desc);
    const sg_resource_state pipeline_state = sg_query_pipeline_state(pipeline_handle);
    if(pipeline_state != SG_RESOURCESTATE_VALID)
        System::Log("Failed to create ground paint pipeline.");

    return std::make_unique<PipelineImpl>(pipeline_handle, shader_handle);
}

void GroundPaintPipeline::Apply(
    IPipeline* pipeline, const IRenderBuffer* position, const IElementBuffer* indices, const GroundPaintDrawParams& params)
{
    pipeline->Apply();

    sg_bindings bindings = {};
    bindings.vertex_buffers[ATTR_POSITION].id = position->Id();
    bindings.index_buffer.id = indices->Id();

    bindings.fs.images[0].id = params.splat_map->Id();
    bindings.fs.samplers[0].id = params.splat_map->SamplerId();

    for(int index = 0; index < GROUND_PAINT_LAYERS; ++index)
    {
        // A layer without a texture is see through, but something has to be bound.
        const ITexture* texture = params.layer_textures[index] ? params.layer_textures[index] : params.splat_map;
        bindings.fs.images[index + 1].id = texture->Id();
        bindings.fs.samplers[index + 1].id = texture->SamplerId();
    }

    sg_apply_bindings(&bindings);
}

void GroundPaintPipeline::SetTransforms(const math::Matrix& projection, const math::Matrix& view, const math::Matrix& model)
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

void GroundPaintPipeline::SetParams(const GroundPaintDrawParams& params, float pixels_per_meter)
{
    const math::Vector rect_size = params.splat_rect.top_right - params.splat_rect.bottom_left;

    const float params_block[] = {
        params.splat_rect.bottom_left.x, params.splat_rect.bottom_left.y, rect_size.x, rect_size.y,
        params.layer_scales[1], params.layer_scales[2], params.layer_scales[3], params.layer_scales[4],
        params.layer_scales[0], params.edge_roughness, pixels_per_meter, noise_size_meters,
        params.blend_layers ? 1.0f : 0.0f, params.layer_textures[0] ? 1.0f : 0.0f, 0.0f, 0.0f,
        params.layer_textures[1] ? 1.0f : 0.0f,
        params.layer_textures[2] ? 1.0f : 0.0f,
        params.layer_textures[3] ? 1.0f : 0.0f,
        params.layer_textures[4] ? 1.0f : 0.0f,
    };

    sg_apply_uniforms(SG_SHADERSTAGE_FS, U_PARAMS_BLOCK, { params_block, sizeof(params_block) });
}

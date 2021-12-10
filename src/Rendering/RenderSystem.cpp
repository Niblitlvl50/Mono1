
#include "RenderSystem.h"
#include "Rendering/Text/TextFunctions.h"
#include "Rendering/Sprite/SpriteFactory.h"
#include "Rendering/Sprite/SpriteSystem.h"
#include "Rendering/Texture/ITextureFactory.h"
#include "Rendering/Texture/TextureFactoryImpl.h"

#include "System/System.h"

#include "imgui/imgui.h"

#ifdef __APPLE__
    #define SOKOL_ASSERT(c) if(!(c)) __builtin_trap();
#endif

#define SOKOL_TRACE_HOOKS
#define SOKOL_GLCORE33
#define SOKOL_GFX_IMPL
#define SOKOL_DEBUG
#include "sokol/sokol_gfx.h"

#define SOKOL_IMGUI_IMPL
#define SOKOL_IMGUI_NO_SOKOL_APP
#include "sokol/sokol_imgui.h"

#include <cstdio>
#include <stdexcept>


namespace
{
    float g_pixels_per_meter = 1.0f;
    const char* g_light_mask_texture = nullptr;
    const char* g_sprite_shadow_texture = nullptr;
    const System::IWindow* g_window = nullptr;

    const mono::ISpriteFactory* g_sprite_factory = nullptr;
    const mono::ITextureFactory* g_texture_factory = nullptr;

    void fail_buffer(sg_buffer buf_id, void* user_data)
    {
        System::Log("RenderSystem|fail_buffer.");
    }
    void fail_image(sg_image img_id, void* user_data)
    {
        System::Log("RenderSystem|fail_image.");
    }
    void fail_shader(sg_shader shd_id, void* user_data)
    {
        System::Log("RenderSystem|fail_shader.");
    }
    void fail_pipeline(sg_pipeline pip_id, void* user_data)
    {
        System::Log("RenderSystem|fail_pipeline.");
    }
    void fail_pass(sg_pass pass_id, void* user_data)
    {
        System::Log("RenderSystem|fail_pass.");
    }
    void err_pass_invalid(void* user_data)
    {
        System::Log("RenderSystem|err_pass_invalid.");
    }
    void err_draw_invalid(void* user_data)
    {
        System::Log("RenderSystem|err_draw_invalid.");
    }
    void err_bindings_invalid(void* user_data)
    {
        System::Log("RenderSystem|err_bindings_invalid.");
    }
}

void mono::InitializeRender(const RenderInitParams& init_params)
{
    sg_desc desc = {};
    desc.buffer_pool_size = 2048;
    desc.context.depth_format = SG_PIXELFORMAT_NONE;
    sg_setup(&desc);

    sg_trace_hooks trace_hooks = {};
    trace_hooks.fail_buffer = fail_buffer;
    trace_hooks.fail_image = fail_image;
    trace_hooks.fail_pass = fail_pass;
    trace_hooks.fail_pipeline = fail_pipeline;
    trace_hooks.fail_shader = fail_shader;
    trace_hooks.err_pass_invalid = err_pass_invalid;
    trace_hooks.err_draw_invalid = err_draw_invalid;
    trace_hooks.err_bindings_invalid = err_bindings_invalid;
    sg_install_trace_hooks(&trace_hooks);

    simgui_desc_t imgui_desc = {};
#ifdef __APPLE__
    imgui_desc.dpi_scale = 2.0f; // could be 2.0f for retina mac
#else
    imgui_desc.dpi_scale = 1.0f;
#endif
    imgui_desc.ini_filename = init_params.imgui_ini;
    simgui_setup(&imgui_desc);

    g_pixels_per_meter = init_params.pixels_per_meter;
    g_light_mask_texture = init_params.light_mask_texture;
    g_sprite_shadow_texture = init_params.sprite_shadow_texture;
    g_window = init_params.window;

    g_sprite_factory = new SpriteFactoryImpl(init_params.pixels_per_meter);
    g_texture_factory = new TextureFactoryImpl();

    // glEnable(GL_POINT_SMOOTH);
    // glEnable(GL_LINE_SMOOTH);
    // const GLubyte* vendor = glGetString(GL_VENDOR);
    // const GLubyte* renderer = glGetString(GL_RENDERER);
    // const GLubyte* version = glGetString(GL_VERSION);
    // const GLubyte* glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    //const GLubyte* extensions = glGetString(GL_EXTENSIONS);

    System::Log("Render\n"
                "\tpixels per meter: %f", init_params.pixels_per_meter);
    //System::Log("OpenGL\n"
    //            "\tvendor: %s\n"
    //            "\trenderer: %s\n"
    //            "\tversion: %s\n"
    //            "\tglsl: %s", vendor, renderer, version, glslversion);
    System::Log("imgui\n"
                "\tversion: %s", IMGUI_VERSION);
}

void mono::ShutdownRender()
{
    delete g_sprite_factory;
    g_sprite_factory = nullptr;

    delete g_texture_factory;
    g_texture_factory = nullptr;

    mono::UnloadFonts();

    simgui_shutdown();
    sg_shutdown();
}

float mono::PixelsPerMeter()
{
    return g_pixels_per_meter;
}

float mono::GetWindowAspect()
{
    System::Size window_size = g_window->Size();
    return float(window_size.width) / float(window_size.height);
}

const char* mono::LightMaskTexture()
{
    return g_light_mask_texture;
}

const char* mono::SpriteShadowTexture()
{
    return g_sprite_shadow_texture;
}

void mono::LoadCustomTextureFactory(const ITextureFactory* texture_factory)
{
    if(g_texture_factory)
        delete g_texture_factory;

    g_texture_factory = texture_factory;
}

const mono::ISpriteFactory* mono::GetSpriteFactory()
{
    return g_sprite_factory;
}

const mono::ITextureFactory* mono::GetTextureFactory()
{
    return g_texture_factory;
}

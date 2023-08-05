
#include "RenderSystem.h"
#include "Rendering/Text/TextFunctions.h"
#include "Rendering/Sprite/SpriteFactory.h"
#include "Rendering/Sprite/SpriteSystem.h"
#include "Rendering/Texture/ITextureFactory.h"
#include "Rendering/Texture/TextureFactoryImpl.h"

#include "Math/EasingFunctions.h"

#include "System/System.h"
#include "System/Debug.h"

#include "imgui/imgui.h"

#define SOKOL_ASSERT(c) MONO_ASSERT(c)

#define SOKOL_TRACE_HOOKS
#define SOKOL_GLCORE33
#define SOKOL_IMPL
#define SOKOL_DEBUG
#include "sokol/sokol_gfx.h"

#define SOKOL_IMGUI_IMPL
#define SOKOL_IMGUI_NO_SOKOL_APP
#include "sokol/sokol_imgui.h"

#include <cstdio>
#include <stdexcept>

namespace
{
    void fail_buffer(sg_buffer buf_id, void* user_data)
    {
        MONO_ASSERT_MESSAGE(false, "RenderSystem|fail_buffer.");
    }
    void fail_image(sg_image img_id, void* user_data)
    {
        MONO_ASSERT_MESSAGE(false, "RenderSystem|fail_image.");
    }
    void fail_sampler(sg_sampler smp_id, void* user_data)
    {
        MONO_ASSERT_MESSAGE(false, "RenderSystem|fail_sampler.");
    }
    void fail_shader(sg_shader shd_id, void* user_data)
    {
        MONO_ASSERT_MESSAGE(false, "RenderSystem|fail_shader.");
    }
    void fail_pipeline(sg_pipeline pip_id, void* user_data)
    {
        MONO_ASSERT_MESSAGE(false, "RenderSystem|fail_pipeline.");
    }
    void fail_pass(sg_pass pass_id, void* user_data)
    {
        MONO_ASSERT_MESSAGE(false, "RenderSystem|fail_pass.");
    }

    void sokol_gfx_logger(
        const char* tag,                // always "sg"
        uint32_t log_level,             // 0=panic, 1=error, 2=warning, 3=info
        uint32_t log_item_id,           // SG_LOGITEM_*
        const char* message_or_null,    // a message string, may be nullptr in release mode
        uint32_t line_nr,               // line number in sokol_gfx.h
        const char* filename_or_null,   // source filename, may be nullptr in release mode
        void* user_data)
    {
        MONO_ASSERT_MESSAGE(log_level >= 3, message_or_null);
    }

    void sokol_imgui_logger(
        const char* tag,                // always "simgui"
        uint32_t log_level,             // 0=panic, 1=error, 2=warning, 3=info
        uint32_t log_item_id,           // SIMGUI_LOGITEM_*
        const char* message_or_null,    // a message string, may be nullptr in release mode
        uint32_t line_nr,               // line number in sokol_imgui.h
        const char* filename_or_null,   // source filename, may be nullptr in release mode
        void* user_data)
    {
        MONO_ASSERT_MESSAGE(log_level >= 3, message_or_null);
    }    
}

using namespace mono;

float RenderSystem::s_pixels_per_meter = 32.0f;
const char* RenderSystem::s_light_mask_texture = nullptr;
const char* RenderSystem::s_sprite_shadow_texture = nullptr;
const System::IWindow* RenderSystem::s_window = nullptr;
const mono::ISpriteFactory* RenderSystem::s_sprite_factory = nullptr;
const mono::ITextureFactory* RenderSystem::s_texture_factory = nullptr;

RenderSystem::RenderSystem(uint32_t n, const RenderInitParams& init_params)
    : m_layers(n)
{
    sg_desc desc = {};
    desc.buffer_pool_size = 2048;
    desc.logger.func = sokol_gfx_logger;
    sg_setup(&desc);

    sg_trace_hooks trace_hooks = {};
    trace_hooks.fail_buffer = fail_buffer;
    trace_hooks.fail_image = fail_image;
    trace_hooks.fail_sampler = fail_sampler;
    trace_hooks.fail_shader = fail_shader;
    trace_hooks.fail_pipeline = fail_pipeline;
    trace_hooks.fail_pass = fail_pass;
    sg_install_trace_hooks(&trace_hooks);

    simgui_desc_t imgui_desc = {};
    imgui_desc.ini_filename = init_params.imgui_ini;
    imgui_desc.image_pool_size = 512;
    imgui_desc.logger = { sokol_imgui_logger, nullptr };
    simgui_setup(&imgui_desc);

    s_pixels_per_meter = init_params.pixels_per_meter;
    s_light_mask_texture = init_params.light_mask_texture;
    s_sprite_shadow_texture = init_params.sprite_shadow_texture;
    s_window = init_params.window;

    s_sprite_factory = new SpriteFactoryImpl(init_params.pixels_per_meter);
    s_texture_factory = new TextureFactoryImpl();

    const System::Size window_size = s_window->Size();
    const System::Size drawable_size = s_window->DrawableSize();

    System::Log("Render\n"
                "\tpixels per meter: %f", init_params.pixels_per_meter);
    System::Log("imgui\n"
                "\tversion: %s", IMGUI_VERSION);
    System::Log("Game Window\n"
                "\twindow size w: %d h: %d\n"
                "\tdrawable size w: %d h: %d",
                window_size.width, window_size.height,
                drawable_size.width, drawable_size.height);
}

RenderSystem::~RenderSystem()
{
    delete s_sprite_factory;
    s_sprite_factory = nullptr;

    delete s_texture_factory;
    s_texture_factory = nullptr;

    mono::UnloadFonts();
    simgui_shutdown();
    sg_shutdown();
}

const char* RenderSystem::Name() const
{
    return "rendersystem";
}

void RenderSystem::Update(const mono::UpdateContext& update_context)
{
    const bool process_screenfade = m_screen_fade.has_value();
    if(process_screenfade)
    {
        ScreenFadeData& screen_fade = m_screen_fade.value();

        if(screen_fade.state == ScreenFadeState::FADE_IN)
            screen_fade.alpha = math::EaseOutCubic(screen_fade.timer_s, screen_fade.duration_s, 0.0f, 1.0f);
        else if(screen_fade.state == ScreenFadeState::FADE_OUT || screen_fade.state == ScreenFadeState::FADE_OUT_PAUSE_IN)
            screen_fade.alpha = math::EaseOutCubic(screen_fade.timer_s, screen_fade.duration_s, 1.0f, -1.0f);

        screen_fade.timer_s += update_context.delta_s;

        if(screen_fade.timer_s >= screen_fade.duration_s)
        {
            if(screen_fade.state == ScreenFadeState::FADE_OUT_PAUSE_IN)
            {
                screen_fade.timer_s = 0.0f;
                screen_fade.duration_s = screen_fade.pause_duration_s;
                screen_fade.state = ScreenFadeState::FADE_PAUSE;

                if(screen_fade.callback)
                    screen_fade.callback(ScreenFadeState::FADE_OUT);
            }
            else if(screen_fade.state == ScreenFadeState::FADE_PAUSE)
            {
                screen_fade.timer_s = 0.0f;
                screen_fade.duration_s = screen_fade.fade_duration_s;
                screen_fade.state = ScreenFadeState::FADE_IN;

                if(screen_fade.callback)
                    screen_fade.callback(ScreenFadeState::FADE_PAUSE);
            }
            else
            {
                if(screen_fade.callback)
                    screen_fade.callback(ScreenFadeState::FADE_IN);

                m_screen_fade.reset();
            }
        }
    }
}

void RenderSystem::AllocateLayer(uint32_t entity_id)
{
    LayerComponent component;
    component.layer = 0;

    m_layers.Set(entity_id, std::move(component));
}

void RenderSystem::ReleaseLayer(uint32_t entity_id)
{
    m_layers.Release(entity_id);
}

void RenderSystem::UpdateLayer(uint32_t entity_id, int new_layer, float new_sort_offset)
{
    LayerComponent* component = m_layers.Get(entity_id);
    component->layer = new_layer;
    component->sort_offset = new_sort_offset;
}

int RenderSystem::GetRenderLayerOrDefault(uint32_t entity_id) const
{
    const bool is_active = m_layers.IsActive(entity_id);
    if(is_active)
    {
        const LayerComponent* component = m_layers.Get(entity_id);
        return component->layer;
    }

    return 0;
}

float RenderSystem::GetRenderSortOffsetOrDefault(uint32_t entity_id) const
{
    const bool is_active = m_layers.IsActive(entity_id);
    if(is_active)
    {
        const LayerComponent* component = m_layers.Get(entity_id);
        return component->sort_offset;
    }

    return 0.0f;
}

void RenderSystem::TriggerScreenFade(
    ScreenFadeState state, float fade_time, float pause_time, ScreenFadeCallback callback)
{
    MONO_ASSERT(!m_screen_fade.has_value());

    ScreenFadeData screenfade_data;
    screenfade_data.state = state;
    screenfade_data.fade_duration_s = fade_time;
    screenfade_data.pause_duration_s = pause_time;
    screenfade_data.callback = callback;
    screenfade_data.duration_s = fade_time;
    screenfade_data.timer_s = 0.0f;
    screenfade_data.alpha = (state == ScreenFadeState::FADE_IN) ? 0.0f : 1.0f;
    m_screen_fade.emplace(screenfade_data);
}

bool RenderSystem::ShouldApplyScreenFadeAlpha() const
{
    return m_screen_fade.has_value();
}

float RenderSystem::GetFadeAlpha() const
{
    return m_screen_fade.value().alpha;
}

float RenderSystem::PixelsPerMeter()
{
    return s_pixels_per_meter;
}

float RenderSystem::GetWindowAspect()
{
    const System::Size window_size = s_window->Size();
    return float(window_size.width) / float(window_size.height);
}

const char* RenderSystem::LightMaskTexture()
{
    return s_light_mask_texture;
}

const char* RenderSystem::SpriteShadowTexture()
{
    return s_sprite_shadow_texture;
}

void RenderSystem::LoadCustomTextureFactory(const ITextureFactory* texture_factory)
{
    if(s_texture_factory)
        delete s_texture_factory;

    s_texture_factory = texture_factory;
}

const ISpriteFactory* RenderSystem::GetSpriteFactory()
{
    return s_sprite_factory;
}

const ITextureFactory* RenderSystem::GetTextureFactory()
{
    return s_texture_factory;
}

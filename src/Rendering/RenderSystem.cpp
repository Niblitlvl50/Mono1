
#include "RenderSystem.h"
#include "Rendering/Text/TextFunctions.h"
#include "Rendering/Shader/ShaderFactory.h"
#include "Rendering/Sprite/SpriteFactory.h"
#include "Rendering/Sprite/SpriteSystem.h"

#include "System/System.h"
#include <cstdio>

const mono::IShaderFactory* g_shader_factory = nullptr;
const mono::ISpriteFactory* g_sprite_factory = nullptr;

void mono::InitializeRender(const RenderInitParams& init_params)
{
    g_shader_factory = new ShaderFactory;
    g_sprite_factory = new SpriteFactoryImpl(init_params.pixels_per_meter);

    System::Log("Render\n"
                "\tpixels per meter: %f\n", init_params.pixels_per_meter);
}

void mono::ShutdownRender()
{
    delete g_shader_factory;
    g_shader_factory = nullptr;

    delete g_sprite_factory;
    g_sprite_factory = nullptr;

    mono::UnloadFonts();
}

void mono::LoadCustomShaderFactory(const mono::IShaderFactory* custom_factory)
{
    if(g_shader_factory)
        delete g_shader_factory;

    g_shader_factory = custom_factory;
}

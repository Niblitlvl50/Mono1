
#include "RenderSystem.h"
#include "Rendering/Text/TextFunctions.h"
#include "Rendering/Shader/ShaderFactory.h"
#include "Rendering/Sprite/SpriteFactory.h"
#include "Rendering/Sprite/SpriteSystem.h"
#include "Rendering/Texture/ITextureFactory.h"
#include "Rendering/Texture/TextureFactoryImpl.h"

#include "System/System.h"
#include <cstdio>

const mono::IShaderFactory* g_shader_factory = nullptr;
const mono::ISpriteFactory* g_sprite_factory = nullptr;
const mono::ITextureFactory* g_texture_factory = nullptr;

void mono::InitializeRender(const RenderInitParams& init_params)
{
    g_shader_factory = new ShaderFactory;
    g_sprite_factory = new SpriteFactoryImpl(init_params.pixels_per_meter);
    g_texture_factory = new TextureFactoryImpl();

    System::Log("Render\n"
                "\tpixels per meter: %f\n", init_params.pixels_per_meter);
}

void mono::ShutdownRender()
{
    delete g_shader_factory;
    g_shader_factory = nullptr;

    delete g_sprite_factory;
    g_sprite_factory = nullptr;

    delete g_texture_factory;
    g_texture_factory = nullptr;

    mono::UnloadFonts();
}

void mono::LoadCustomShaderFactory(const mono::IShaderFactory* custom_factory)
{
    if(g_shader_factory)
        delete g_shader_factory;

    g_shader_factory = custom_factory;
}

void mono::LoadCustomTextureFactory(const ITextureFactory* texture_factory)
{
    if(g_texture_factory)
        delete g_texture_factory;

    g_texture_factory = texture_factory;
}


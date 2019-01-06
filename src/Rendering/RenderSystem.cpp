
#include "RenderSystem.h"

#include "Rendering/Text/TextFunctions.h"
#include "Rendering/Shader/ShaderFactory.h"
#include "Rendering/Sprite/SpriteFactory.h"
#include "Rendering/Sprite/SpriteInstanceCollection.h"

#include <cstdio>

const mono::IShaderFactory* g_shader_factory = nullptr;
mono::SpriteInstanceCollection* g_sprite_instance_collection = nullptr;
const mono::ISpriteFactory* g_sprite_factory = nullptr;

void mono::InitializeRender(const RenderInitParams& init_params)
{
    g_shader_factory = new ShaderFactory;
    g_sprite_instance_collection = new SpriteInstanceCollection(init_params.n_sprites);
    g_sprite_factory = new SpriteFactoryImpl(g_sprite_instance_collection, init_params.pixels_per_meter);

    std::printf("Render\n"
                "\tpixels per meter: %f\n"
                "\tsprites: %zu\n", init_params.pixels_per_meter, init_params.n_sprites);
}

void mono::ShutdownRender()
{
    delete g_shader_factory;
    g_shader_factory = nullptr;

    delete g_sprite_factory;
    g_sprite_factory = nullptr;

    delete g_sprite_instance_collection;
    g_sprite_instance_collection = nullptr;

    mono::UnloadFonts();
}

void mono::LoadCustomShaderFactory(const mono::IShaderFactory* custom_factory)
{
    if(g_shader_factory)
        delete g_shader_factory;

    g_shader_factory = custom_factory;
}

const mono::SpriteInstanceCollection* mono::GetSpriteInstances()
{
    return g_sprite_instance_collection;
}

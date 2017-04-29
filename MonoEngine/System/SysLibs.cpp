
#include "SysLibs.h"
#include "SysSound.h"
#include "SysNetwork.h"
#include "Rendering/Text/TextFunctions.h"
#include "Rendering/Shader/ShaderFunctions.h"
#include "SDL.h"

#include <stdexcept>
#include <cstdio>
#include <string>

void System::Init()
{
    // Init SDL video subsystem
    const int result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    if(result != 0)
        throw std::runtime_error("Couldn't initialize libSDL" + std::string(SDL_GetError()));

    SDL_version version;
    SDL_GetVersion(&version);

    std::printf("SDL\n");
    std::printf("\tversion: %u.%u.%u\n", version.major, version.minor, version.patch);

    Sound::Init();
    Network::Init();
    mono::LoadDefaultShaderFactory();
}

void System::Exit()
{
    mono::UnloadShaderFactory();
    mono::UnloadFonts();
    Sound::Exit();
    Network::Exit();

    SDL_Quit();
}

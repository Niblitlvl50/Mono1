
#include "System/System.h"
#include "Rendering/RenderSystem.h"

#include "Camera/TraceCamera.h"
#include "EventHandler/EventHandler.h"
#include "Rendering/Text/TextFunctions.h"

#include "Engine.h"
#include "Animator.h"

#include <cstdio>

int main(int argc, const char * argv[])
{
    if(argc < 2)
    {
        std::printf("You need to supply an argument\n");
        return 1;
    }

    // This is assumed to be the file argument
    const char* file = argv[1];

    System::Initialize();
    mono::InitializeRender();

    {
        // The "global" event handler used throughout the game
        mono::EventHandler eventHandler;

        System::IWindow* window = System::CreateWindow("Animator", 1200, 800, false);
        window->SetBackgroundColor(0.6, 0.6, 0.6);

        mono::ICameraPtr camera = std::make_shared<mono::TraceCamera>(300, 200);
        mono::LoadFont(0, "pixelette.ttf", 10.0f);

        mono::Engine engine(window, camera, eventHandler);
        engine.Run(std::make_shared<animator::Animator>(window, eventHandler, file));

        delete window;
    }

    mono::ShutdownRender();
    System::Shutdown();
    
    return 0;
}

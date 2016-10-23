//
//  main.cpp
//  AnimationTool
//
//  Created by Niklas Damberg on 28/11/15.
//
//

#include "System/SysLibs.h"
#include "System/WindowFactory.h"
#include "Camera/TraceCamera.h"
#include "Engine.h"
#include "EventHandler/EventHandler.h"

#include "IWindow.h"
#include "Color.h"
#include "Text/TextFunctions.h"
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

    System::Init();

    // The "global" event handler used throughout the game
    mono::EventHandler eventHandler;

    auto window = mono::CreateWindow("Animator", 1200, 800, false);
    window->SetBackgroundColor(mono::Color::RGBA(0.6, 0.6, 0.6));

    mono::ICameraPtr camera = std::make_shared<mono::TraceCamera>(300, 200);
    mono::LoadFont(0, "pixelette.ttf", 10.0f);

    mono::Engine engine(window, camera, eventHandler);
    engine.Run(std::make_shared<animator::Animator>(window, eventHandler, file));

    System::Exit();
    
    return 0;
}

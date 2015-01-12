

#include "SysLibs.h"
#include "SysVideo.h"
#include "WindowFactory.h"
#include "TestZone.h"
#include "TraceCamera.h"
#include "Engine.h"
#include "TextFunctions.h"
#include "Color.h"
#include "EventHandler.h"
#include "IWindow.h"

#include <stdexcept>
#include <iostream>

int main()
{
    try
    {
        Libs::Init();

        // The "global" event handler used throughout the game
        mono::EventHandler eventHandler;

        const math::Vector2f& size = Video::GetCurrentWindowSize(); // / 2;
        mono::IWindowPtr window = mono::CreateWindow("Mono1", size.mX, size.mY, false);
        window->SetBackgroundColor(mono::Color(0.6, 0.6, 0.6, 1.0));

        mono::ICameraPtr camera = std::make_shared<mono::TraceCamera>(size.mX, size.mY, eventHandler);
        mono::LoadFont("pixelette.ttf", 10.0f);

        mono::Engine engine(window, camera, eventHandler);
        engine.Run(std::make_shared<game::TestZone>(eventHandler));
    }
    catch(const std::runtime_error& e)
    {
        std::cout << "Shutdown due to exception: " << e.what() << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cout << "Shutdown due to exception: " << e.what() << std::endl;
    }
    catch(...)
    {
        std::cout << "Shutdown due to unknown exception" << std::endl;
    }

    Libs::Exit();
	
    return 0;
}



#include "SDL.h"

#include "SysLibs.h"
#include "SysVideo.h"
#include "WindowFactory.h"
#include "TestZone.h"
#include "TraceCamera.h"
#include "Engine.h"
#include "TextFunctions.h"

#include <stdexcept>
#include <iostream>

int main(int argc, char* argv[])
{
    try
    {
        Libs::Init();
        
        const math::Vector2f size = Video::GetCurrentWindowSize();
        
        mono::IWindowPtr window = mono::CreateOpenGLWindow("Mono1", size.mX, size.mY, false);
        mono::ICameraPtr camera(new mono::TraceCamera(size.mX / 2.0, size.mY / 2.0));
        mono::LoadFont("pixelette.ttf", 10.0f);

        mono::Engine engine(60, window, camera, mono::IZonePtr(new game::TestZone));
        engine.Run();
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


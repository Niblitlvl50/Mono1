

#include "SDL.h"

#include "SysLibs.h"
#include "WindowFactory.h"
#include "TextFunctions.h"
#include "TraceCamera.h"
#include "Engine.h"
#include "TestZone.h"

#include <stdexcept>
#include <iostream>


namespace
{
    static const int ipadWidth = 768;
    static const int ipadHeight = 1024;
}

int main(int argc, char *argv[])
{
    try
    {
        Libs::Init();
        
        mono::IWindowPtr window = mono::CreateOpenGLWindow("Mono1", ipadHeight, ipadWidth, false);
        mono::LoadFont("pixelette.ttf", 10.0f);
        
        mono::Engine engine(60, window, mono::ICameraPtr(new mono::TraceCamera(400, 250)), mono::IZonePtr(new game::TestZone));
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

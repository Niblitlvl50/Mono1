


#include "SysLibs.h"
#include "WindowFactory.h"
#include "TestZone.h"
#include "TraceCamera.h"
#include "Engine.h"
#include "TextFunctions.h"

#include <stdexcept>
#include <iostream>


int main()
{
    try
    {
        Libs::Init();
                
        mono::IWindowPtr window = mono::CreateOpenGLWindow("Mono1", 1280, 800, true);
        mono::Engine engine(60, window, mono::ICameraPtr(new mono::TraceCamera(800, 500)), mono::IZonePtr(new game::TestZone));
        mono::LoadFont("HelveticaLight.ttf", 14.0f);

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


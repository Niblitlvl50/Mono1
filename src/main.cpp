


#include "Engine.h"
#include "WindowFactory.h"
#include "SysLibs.h"

#include <stdexcept>
#include <iostream>

#include "ICamera.h"
#include "GameController.h"

#include "TestZone.h"
#include "OpenGLSetup.h"

namespace
{
    const int width = 800;
    const int height = 480;
    const bool fullscreen = false;
    
    //const int width = 1280;
    //const int height = 800;
    //const bool fullscreen = true;
    
    struct BasicCamera : public mono::ICamera
    {
        // Dummy...
    };
}

int main()
{
    try
    {
        Libs::Init();
                
        mono::IWindowPtr window = mono::CreateOpenGLWindow(width, height, fullscreen, mono::IOGLSetupPtr(new game::OpenGLSetup));
        mono::ICameraPtr camera(new BasicCamera);
        mono::Engine engine(60.0f, window, camera);
        
        // Do the zone initialization here.
        mono::GameController::Instance().InsertZone(mono::IZonePtr(new game::TestZone));

        engine.Run();
    }
    catch(const std::runtime_error& e)
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


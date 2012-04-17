


#include "Engine.h"
#include "WindowFactory.h"
#include "SysLibs.h"

#include "ICamera.h"
#include "GameController.h"

#include "TestZone.h"

#include <stdexcept>
#include <iostream>

namespace
{        
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
                
        mono::IWindowPtr window = mono::CreateOpenGLWindow("Mono1", 800, 480, false);
        mono::ICameraPtr camera(new BasicCamera);
        mono::Engine engine(60.0f, window, camera);
        
        // Do the zone initialization here.
        mono::GameControllerInstance().InsertZone(mono::IZonePtr(new game::TestZone));

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


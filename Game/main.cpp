
#include "System/System.h"
#include "Audio/AudioSystem.h"
#include "Rendering/RenderSystem.h"

#include "Camera/TraceCamera.h"
#include "Engine.h"
#include "Rendering/Text/TextFunctions.h"
#include "EventHandler/EventHandler.h"

#include "TestZone.h"
#include "Factories.h"
#include "Weapons/WeaponFactory.h"
#include "Enemies/EnemyFactory.h"
#include "FontIds.h"

int main(int argc, char* argv[])
{
    System::Initialize();
    mono::InitializeAudio();
    mono::InitializeRender();

    {
        // The "global" event handler used throughout the game
        mono::EventHandler eventHandler;

        game::weapon_factory = new game::WeaponFactory(eventHandler);
        game::enemy_factory = new game::EnemyFactory(eventHandler);

        //constexpr math::Vector iPhone6SSize(750.0f, 1334.0f);
        //const math::Vector& nativeSize = Video::GetCurrentWindowSize();
        //const float ratio = nativeSize.y / iPhone6SSize.y;

        //const math::Vector& size = iPhone6SSize * ratio;
        const math::Vector size(1280, 800);

        System::IWindow* window = System::CreateWindow("Shmup", size.x, size.y, false);
        window->SetBackgroundColor(0.6, 0.6, 0.6);

        mono::ICameraPtr camera = std::make_shared<mono::TraceCamera>(24, 16);
        
        mono::LoadFont(game::FontId::SMALL,  "pixelette.ttf", 10.0f, 1.0f / 10.0f);
        mono::LoadFont(game::FontId::MEDIUM, "pixelette.ttf", 10.0f, 1.0f / 5.0f);
        mono::LoadFont(game::FontId::LARGE,  "pixelette.ttf", 10.0f, 1.0f);

        mono::Engine engine(window, camera, eventHandler);
        engine.Run(std::make_shared<game::TestZone>(eventHandler));

        delete window;
        delete game::weapon_factory;
        delete game::enemy_factory;
    }

    mono::ShutdownRender();
    mono::ShutdownAudio();
    System::Shutdown();

    return 0;
}


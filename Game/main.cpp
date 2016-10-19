

#include "System/SysLibs.h"
#include "System/SysVideo.h"
#include "System/WindowFactory.h"
#include "TestZone.h"
#include "Camera/TraceCamera.h"
#include "Engine.h"
#include "Text/TextFunctions.h"
#include "Color.h"
#include "EventHandler/EventHandler.h"
#include "IWindow.h"

#include "GameContext.h"
#include "FontIds.h"

int main(int argc, char* argv[])
{
    System::Init();

    // The "global" event handler used throughout the game
    mono::EventHandler eventHandler;

    game::context.event_handler = &eventHandler;

    //constexpr math::Vector2f iPhone6SSize(750.0f, 1334.0f);
    //const math::Vector2f& nativeSize = Video::GetCurrentWindowSize();
    //const float ratio = nativeSize.y / iPhone6SSize.y;

    //const math::Vector2f& size = iPhone6SSize * ratio;
    const math::Vector2f size(1280, 800);

    mono::IWindowPtr window = mono::CreateWindow("Mono1", size.x, size.y, false);
    window->SetBackgroundColor(mono::Color::RGBA(0.6, 0.6, 0.6));

    mono::ICameraPtr camera = std::make_shared<mono::TraceCamera>(size.x / 2, size.y / 2);
    
    mono::LoadFont(game::FontId::SMALL,  "pixelette.ttf", 10.0f, 1.0f / 10.0f);
    mono::LoadFont(game::FontId::MEDIUM, "pixelette.ttf", 10.0f, 1.0f / 5.0f);
    mono::LoadFont(game::FontId::LARGE,  "pixelette.ttf", 10.0f, 1.0f);

    mono::Engine engine(window, camera, eventHandler);
    engine.Run(std::make_shared<game::TestZone>(eventHandler));

    System::Exit();

    return 0;
}


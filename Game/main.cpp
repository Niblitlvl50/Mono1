

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

int main(int argc, char* argv[])
{
    constexpr math::Vector2f iPhone6SSize(750.0f, 1334.0f);

    System::Init();

    // The "global" event handler used throughout the game
    mono::EventHandler eventHandler;

    const math::Vector2f& nativeSize = Video::GetCurrentWindowSize();
    const float ratio = nativeSize.y / iPhone6SSize.y;

    const math::Vector2f& size = iPhone6SSize * ratio;

    mono::IWindowPtr window = mono::CreateWindow("Mono1", size.x, size.y, false);
    window->SetBackgroundColor(mono::Color::RGBA(0.6, 0.6, 0.6));

    mono::ICameraPtr camera = std::make_shared<mono::TraceCamera>(size.x / 2, size.y / 2, eventHandler);
    mono::LoadFont("pixelette.ttf", 10.0f);

    mono::Engine engine(window, camera, eventHandler);
    engine.Run(std::make_shared<game::TestZone>(eventHandler));

    System::Exit();

    return 0;
}


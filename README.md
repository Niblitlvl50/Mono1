Mono1
=====

A SDL 2.0 game engine... taking form. 



Usage

// First init the library
Libs::Init();

// The "global" event handler used throughout the game.
mono::EventHandler eventHandler;

// Setup the shader factory. Done as a global to be able to test stuff.
mono::ShaderFactory factory;
shaderFactory = &factory;

// Define a Vector2f that specifies how large window you want, there is a 
// function for checking the current desktop windows size and that is
// this function: Video::GetCurrentWindowSize().
const math::Vector2f& size(800, 600);

// Create a window, the last parameter is if you want to window fullscreen or not
// although this is disabled for now. 
mono::IWindowPtr window = mono::CreateWindow("Mono1", size.mX, size.mY, false);

// Create a camera, there is a "TraceCamera" included but you can create your own
// by inheriting from the mono::ICamera interface.
mono::ICameraPtr camera = std::make_shared<mono::TraceCamera>(size.mX, size.mY, eventHandler);

// If you want to use text, you need to load a font. MonoEngine supports truetype fonts.
mono::LoadFont("pixelette.ttf", 10.0f);

// Create the engine, pass in the window, camera and the event handler
mono::Engine engine(window, camera, eventHandler);
engine.Run(std::make_shared<game::TestZone>(eventHandler));

// For good practice, call this when you want to clean up the font
mono::UnloadFont();

// And lastly, for good practice as well, exit the library
Libs::Exit();


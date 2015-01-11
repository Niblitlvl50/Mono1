Mono1
=====

A 2D SDL2 game engine


Usage

```C++
// First init the library
Libs::Init();

// Declare an instance of the mono::EventHandler, this is used to pass events 
// between objects.
mono::EventHandler eventHandler;

// Setup the shader factory. Done as a global to be able to test stuff.
mono::ShaderFactory factory;
shaderFactory = &factory;

// Create a window, the last parameter is if you want the window fullscreen or not,
// although this is disabled for now. 
mono::IWindowPtr window = mono::CreateWindow("GameWindow", 800, 600, false);

// Create a camera, there is a "TraceCamera" object in the library but you can create your own
// by inheriting from the mono::ICamera interface.
mono::ICameraPtr camera = std::make_shared<mono::TraceCamera>(800, 600, eventHandler);

// If you want to use text, you need to load a font, and it has to be done after you 
// have created an OpenGL context, in this case after we created the window.
// MonoEngine supports truetype fonts.
mono::LoadFont("pixelette.ttf", 10.0f);

// Create the engine, pass in the window, camera and the event handler
mono::Engine engine(window, camera, eventHandler);

// Then you call the Run function and pass in your own "Zone" object, it's an object
// inheriting from the mono::IZone interface. This is where your game will take place.
engine.Run(std::make_shared<game::GameZone>(eventHandler));

// For good practice, call UnloadFont() when you want to clean up the font.
mono::UnloadFont();

// And lastly, for good practice as well, exit the library.
Libs::Exit();
```

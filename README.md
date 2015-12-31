Mono1
=====

## Usage

```C++
// First init the library
Libs::Init();

// Declare an instance of the mono::EventHandler, this is used to pass events
// between objects.
mono::EventHandler eventHandler;

// Create a window, the last parameter is if you want the window fullscreen or not,
// although this is disabled for now.
mono::IWindowPtr window = mono::CreateWindow("GameWindow", 800, 600, false);

// Create a camera, there is a "TraceCamera" object in the library but you can
// create your own by inheriting from the mono::ICamera interface.
mono::ICameraPtr camera = std::make_shared<mono::TraceCamera>(800, 600, eventHandler);

// If you want to use text, you need to load a font, and it has to be done after
// you have created an OpenGL context, in this case after we created the window.
// MonoEngine supports truetype fonts.
mono::LoadFont("pixelette.ttf", 10.0f);

// Create the engine, pass in the window, camera and the event handler
mono::Engine engine(window, camera, eventHandler);

// Then you call the Run function and pass in your own "Zone" object, it's an
// object inheriting from the mono::IZone interface. This is where your game
// will take place.
engine.Run(std::make_shared<game::GameZone>(eventHandler));

// And lastly and for good practice, exit the library.
Libs::Exit();
```

## Linking

Building on OSX you need to link against the following frameworks:

* AudioToolbox.framework
* Carbon.framework
* Cocoa.framework
* CoreAudio.framework
* ForceFeedback.framework
* IOKit.framework
* OpenGL.framework

Building on iOS, link against the following frameworks:

* AudioToolbox.framework
* CoreAudio.framework
* CoreGraphics.framework
* Foundation.framework
* OpenGLES.framework
* QuartzCore.framework
* UIKit.framework


#pragma once

#include "Keycodes.h"
#include <cstdint>

namespace System
{
    struct Size
    {
        int width;
        int height;
    };

    class IWindow
    {
    public:

        virtual ~IWindow()
        { }

        virtual void SurfaceChanged(int width, int height) = 0;
        virtual void Maximize() = 0;
        virtual void Minimize() = 0;
        virtual void RestoreSize() = 0;
        virtual void SwapBuffers() const = 0;
        virtual void MakeCurrent() = 0;
        virtual void SetBackgroundColor(float red, float green, float blue) = 0;
        virtual const System::Size& Size() const = 0;
    };

    class IInputHandler
    {
    public:

        virtual ~IInputHandler()
        { }

        virtual void OnKeyDown(Keycode key, bool ctrl, bool shift, bool alt) = 0;
        virtual void OnKeyUp(Keycode key, bool ctrl, bool shift, bool alt) = 0;
        virtual void OnTextInput(const char* text) = 0;
        virtual void OnMouseDown(MouseButton button, int x, int y) = 0;
        virtual void OnMouseUp(MouseButton button, int x, int y) = 0;
        virtual void OnMouseMotion(int x, int y) = 0;
        virtual void OnMouseWheel(int x, int y) = 0;
        virtual void OnTouchDown(int64_t touchId, float x, float y, float dx, float dy) = 0;
        virtual void OnTouchUp(int64_t touchId, float x, float y, float dx, float dy) = 0;
        virtual void OnTouchMotion(int64_t touchId, float x, float y, float dx, float dy) = 0;
        virtual void OnMultiGesture(float x, float y, float theta, float distance) = 0;
        virtual void OnUserEvent(int code, void* data1, void* data2) = 0;

        virtual void OnAppTerminating() = 0;
        virtual void OnEnterBackground() = 0;
        virtual void OnEnterForeground() = 0;
        virtual void OnQuit() = 0;

        virtual void OnSurfaceChanged(int width, int height) = 0;
        virtual void OnActivated(bool gain) = 0;
    };

    class ITimer
    {
    public:

        virtual ~ITimer()
        { }

        virtual void Start() = 0;
        virtual void Stop() = 0;
    };

    void Initialize();
    void Shutdown();

    unsigned int GetMilliseconds();
    void Sleep(unsigned int ms);

    using timer_callback_t = void (*)(void*);

    //! Create a timer with a single callback after a certain time
    //! The caller is responsible for deleting the pointer
    ITimer* CreateOneShotTimer(unsigned int ms, timer_callback_t callback, void* data);

    //! Creates a timer with repeating callbacks at a certain intervall
    //! The caller is responsible for deleting the pointer
    ITimer* CreateRepeatingTimer(unsigned int ms, timer_callback_t callback, void* data);

    Size GetCurrentWindowSize();

    //! Creates a opengl window
    //! The caller is responsible for deleting the pointer
    IWindow* CreateWindow(const char* title, int width, int height, bool fullscreen);

    //! Process the system events, input and other events
    void ProcessSystemEvents(IInputHandler* handler);

    int KeycodeToNative(Keycode key);
}

namespace Controller
{
    struct State
    {
        int id = -1;
        const char* name = nullptr;

        bool a = false;
        bool b = false;
        bool x = false;
        bool y = false;

        bool left_shoulder  = false;
        bool right_shoulder = false;

        bool left_stick  = false;
        bool right_stick = false;

        bool back = false;
        bool guide = false;
        bool start = false;

        bool up    = false;
        bool down  = false;
        bool left  = false;
        bool right = false;

        float left_x    = 0.0f;
        float left_y    = 0.0f;
        float right_x   = 0.0f;
        float right_y   = 0.0f;

        float left_trigger  = 0.0f;
        float right_trigger = 0.0f;
    };

    constexpr int num_states = 2;
    extern State states[num_states];

    void Initialize();
    void Shutdown();
    void ProcessControllerState();
}

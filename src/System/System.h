
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

    struct Position
    {
        int x;
        int y;
    };

    struct ControllerState
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
        virtual const System::Position& Position() const = 0;
        virtual const System::Size& Size() const = 0;
    };

    class IInputHandler
    {
    public:

        virtual ~IInputHandler()
        { }

        virtual void OnKeyDown(Keycode key, bool ctrl, bool shift, bool alt) = 0;
        virtual void OnKeyUp(Keycode key, bool ctrl, bool shift, bool alt) = 0;

        virtual void OnMouseDown(MouseButton button, int x, int y) = 0;
        virtual void OnMouseUp(MouseButton button, int x, int y) = 0;
        virtual void OnMouseMotion(int x, int y) = 0;
        virtual void OnMouseWheel(int x, int y) = 0;

        virtual void OnTouchDown(int64_t touchId, float x, float y, float dx, float dy) = 0;
        virtual void OnTouchUp(int64_t touchId, float x, float y, float dx, float dy) = 0;
        virtual void OnTouchMotion(int64_t touchId, float x, float y, float dx, float dy) = 0;
        virtual void OnMultiGesture(float x, float y, float theta, float distance) = 0;

        virtual void OnControllerAdded(int id) = 0;
        virtual void OnControllerRemoved(int id) = 0;

        virtual void OnTextInput(const char* text) = 0;

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

    using timer_callback_t = void (*)(void*);

    void Initialize();
    void Shutdown();

    unsigned int GetMilliseconds();
    void Sleep(unsigned int ms);

    // The caller is responsible for deleting the pointer
    ITimer* CreateTimer(unsigned int ms, bool one_shot, timer_callback_t callback, void* data);

    // Gets the devices window size
    Size GetCurrentWindowSize();

    // Creates a opengl window
    // The caller is responsible for deleting the pointer
    IWindow* CreateWindow(const char* title, int x, int y, int width, int height, bool fullscreen);

    enum class CursorVisibility
    {
        Hidden,
        Shown
    };

    void SetCursorVisibility(CursorVisibility visibility_state);

    // Process the system events, input and other events
    void ProcessSystemEvents(IInputHandler* handler);
    void ProcessControllerState();

    int KeycodeToNative(Keycode key);

    enum class ControllerId : int
    {
        Primary,
        Secondary
    };

    const ControllerState& GetController(ControllerId controller_id);
    bool IsControllerActive(ControllerId controller_id);
}

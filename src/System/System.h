
#pragma once

#include "Keycodes.h"
#include <cstdint>

#define ENUM_BIT(n) (1 << (n))

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
        int id;

        float left_x;
        float left_y;
        float right_x;
        float right_y;

        float left_trigger;
        float right_trigger;

        uint16_t button_state;
    };

    //
    //         Y
    //       X   B
    //         A
    //
    enum ControllerButton
    {
        A               = ENUM_BIT(0),
        B               = ENUM_BIT(1),
        X               = ENUM_BIT(2),
        Y               = ENUM_BIT(3),

        LEFT_SHOULDER   = ENUM_BIT(4),
        RIGHT_SHOULDER  = ENUM_BIT(5),

        LEFT_STICK      = ENUM_BIT(6),
        RIGHT_STICK     = ENUM_BIT(7),

        BACK            = ENUM_BIT(8),
        GUIDE           = ENUM_BIT(9),
        START           = ENUM_BIT(10),

        UP              = ENUM_BIT(11),
        DOWN            = ENUM_BIT(12),
        LEFT            = ENUM_BIT(13),
        RIGHT           = ENUM_BIT(14),

        UNUSED          = ENUM_BIT(15)
    };

    class IWindow
    {
    public:

        virtual ~IWindow() = default;
        virtual void Maximize() = 0;
        virtual void Minimize() = 0;
        virtual void RestoreSize() = 0;
        virtual void SwapBuffers() const = 0;
        virtual void MakeCurrent() = 0;
        virtual System::Position Position() const = 0;
        virtual System::Size Size() const = 0;
        virtual System::Size DrawableSize() const = 0;
    };

    class IInputHandler
    {
    public:

        virtual ~IInputHandler() = default;

        virtual void OnKeyDown(Keycode key, bool ctrl, bool shift, bool alt, bool super) = 0;
        virtual void OnKeyUp(Keycode key, bool ctrl, bool shift, bool alt, bool super) = 0;

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

        virtual ~ITimer() = default;
        virtual void Start() = 0;
        virtual void Stop() = 0;
    };

    using timer_callback_t = void (*)(void* context);

    struct InitializeContext
    {
        const char* working_directory = nullptr;
        const char* log_file = nullptr;
    };

    void Initialize(const InitializeContext& context);
    void Shutdown();

    void Log(const char* fmt, ...);
    
    uint32_t GetMilliseconds();
    uint64_t GetPerformanceCounter();
    uint64_t GetPerformanceFrequency();

    using ProcLoadFunc = void* (*)(const char* name);
    ProcLoadFunc GetProcLoader();

    void Sleep(uint32_t ms);

    // The caller is responsible for deleting the pointer
    ITimer* CreateTimer(uint32_t ms, bool one_shot, timer_callback_t callback, void* context);

    // Gets the devices window size
    Size GetCurrentWindowSize();

    enum WindowOptions
    {
        NONE = 0,
        FULLSCREEN = 1,
        DISABLE_VSYNC = 2,
        BORDERLESS = 4,
    };

    // Creates a opengl window
    // The caller is responsible for deleting the pointer
    IWindow* MakeWindow(const char* title, int x, int y, int width, int height, WindowOptions options);

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
    int GetControllerId(ControllerId controller_id);

    inline bool IsButtonDown(uint16_t button_state, ControllerButton button_bit)
    {
        return (button_state & button_bit);
    }

    inline bool IsButtonTriggered(uint16_t last_button_state, uint16_t current_button_state, ControllerButton button_bit)
    {
        return (!IsButtonDown(last_button_state, button_bit) && IsButtonDown(current_button_state, button_bit));
    }

    inline bool HasButtonChanged(uint16_t last_button_state, uint16_t current_button_state, ControllerButton button_bit)
    {
        return IsButtonDown(last_button_state, button_bit) != IsButtonDown(current_button_state, button_bit);
    }

    inline bool ButtonTriggeredAndChanged(uint16_t last_button_state, uint16_t current_button_state, ControllerButton button_bit)
    {
        return 
            IsButtonTriggered(last_button_state, current_button_state, button_bit) &&
            HasButtonChanged(last_button_state, current_button_state, button_bit);
    }
}

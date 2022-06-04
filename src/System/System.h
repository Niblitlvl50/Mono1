
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

    enum ControllerButton
    {
        FACE_BOTTOM     = ENUM_BIT(0),
        FACE_RIGHT      = ENUM_BIT(1),
        FACE_LEFT       = ENUM_BIT(2),
        FACE_TOP        = ENUM_BIT(3),

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

        UNUSED          = ENUM_BIT(15),

        FACE_ANY        = FACE_BOTTOM | FACE_RIGHT | FACE_LEFT | FACE_TOP
    };

    enum WindowOptions
    {
        NONE            = 0,
        FULLSCREEN      = ENUM_BIT(0),
        DISABLE_VSYNC   = ENUM_BIT(1),
        BORDERLESS      = ENUM_BIT(2),
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
        virtual System::WindowOptions Options() const = 0;
    };

    class IInputHandler
    {
    public:

        virtual ~IInputHandler() = default;

        virtual void OnKeyDown(Keycode key, bool ctrl, bool shift, bool alt, bool super) = 0;
        virtual void OnKeyUp(Keycode key, bool ctrl, bool shift, bool alt, bool super) = 0;

        virtual void OnMouseDown(MouseButton button, int x, int y, bool ctrl, bool shift, bool alt, bool super) = 0;
        virtual void OnMouseUp(MouseButton button, int x, int y, bool ctrl, bool shift, bool alt, bool super) = 0;
        virtual void OnMouseMotion(int x, int y, bool ctrl, bool shift, bool alt, bool super) = 0;
        virtual void OnMouseWheel(int x, int y, bool ctrl, bool shift, bool alt, bool super) = 0;

        virtual void OnTouchDown(int64_t touchId, float x, float y, float dx, float dy) = 0;
        virtual void OnTouchUp(int64_t touchId, float x, float y, float dx, float dy) = 0;
        virtual void OnTouchMotion(int64_t touchId, float x, float y, float dx, float dy) = 0;
        virtual void OnMultiGesture(float x, float y, float theta, float distance) = 0;

        virtual void OnControllerAdded(int controller_id) = 0;
        virtual void OnControllerRemoved(int controller_id) = 0;
        virtual void OnControllerButtonDown(int controller_id, System::ControllerButton button) = 0;

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
    void GetApplicationPath(char* buffer, uint32_t buffer_size);
    void GetUserPath(char* buffer, uint32_t buffer_size);

    void Sleep(uint32_t ms);

    enum TimerProperties
    {
        TP_NONE     = 0,
        ONE_SHOT    = ENUM_BIT(0),
        AUTO_DELETE = ENUM_BIT(1)
    };

    // The caller is responsible for deleting the pointer, unless the properties is set to AUTO_DELETE.
    ITimer* CreateTimer(uint32_t ms, uint32_t properties, timer_callback_t callback, void* context);

    // Gets the devices window size
    Size GetCurrentWindowSize();

    // Creates a opengl window
    // The caller is responsible for deleting the pointer
    IWindow* MakeWindow(const char* title, int x, int y, int width, int height, WindowOptions options);

    enum class CursorVisibility
    {
        Hidden,
        Shown
    };

    void SetCursorVisibility(CursorVisibility visibility_state);

    // Get the clipboard text, it will return how many chars it filled in the buffer.
    uint32_t GetClipboardText(char* output_buffer, uint32_t buffer_size);

    // Process the system events, input and other events
    void ProcessSystemEvents(IInputHandler* handler);
    void ProcessControllerState();

    struct ModifierState
    {
        bool ctrl;
        bool shift;
        bool alt;
        bool super;
    };
    
    ModifierState GetModifierState();

    int KeycodeToNative(Keycode key);

    enum ControllerId
    {
        Primary,
        Secondary,
        Third,
        Fourth,
        N_Controllers
    };

    const ControllerState& GetController(ControllerId controller_id);
    bool IsControllerActive(ControllerId controller_id);
    int GetControllerId(ControllerId controller_id);

    void PlayRumble(ControllerId controller_id, float strength, uint32_t duration_ms);

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

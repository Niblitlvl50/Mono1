
#include "System.h"
#include "Keycodes.h"

#include "SDL.h"
#include "SDL_gamecontroller.h"
#include "SDL_haptic.h"
#include "SDL_keycode.h"
#include "SDL_log.h"
#include "SDL_clipboard.h"

#include <stdexcept>
#include <cstdio>
#include <string>
#include <cstring>
#include <cmath>
#include <limits>

#ifdef _WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif

#undef DELETE

namespace
{
    System::ControllerState g_controller_states[System::ControllerId::N_Controllers];
    SDL_Haptic* g_controller_haptic[System::ControllerId::N_Controllers];

    FILE* g_log_file = nullptr;

    enum PredefinedUserEventCode
    {
        TIMER_CALLBACK = 1
    };

    class SDLWindow : public System::IWindow
    {
    public:

        SDLWindow(const char* title, int x, int y, int width, int height, System::WindowOptions options)
        {
            // Request opengl 2.1 context.
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
            SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

            SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
            SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);

            const uint32_t flags =
                SDL_WINDOW_SHOWN |
                SDL_WINDOW_OPENGL |
                SDL_WINDOW_RESIZABLE |
                SDL_WINDOW_ALLOW_HIGHDPI |
                (options & System::WindowOptions::BORDERLESS ? SDL_WINDOW_BORDERLESS : 0);

            // Create our window centered and with the given resolution
            m_window = SDL_CreateWindow(title, x, y, width, height, flags);
            if(m_window == 0)
            {
                const std::string sdl_error = SDL_GetError();
                throw std::runtime_error("System|Unable to create SDL window. (" + sdl_error + ")");
            }

            m_context = SDL_GL_CreateContext(m_window);
            if(!m_context)
            {
                const std::string sdl_error = SDL_GetError();
                throw std::runtime_error("System|Unable to create OpenGL context. (" + sdl_error + ")");
            }

            const int vsync_value = (options & System::WindowOptions::DISABLE_VSYNC) ? 0 : 1;
            SDL_GL_SetSwapInterval(vsync_value);

            MakeCurrent();
        }
        ~SDLWindow()
        {
            SDL_GL_DeleteContext(m_context);
            SDL_DestroyWindow(m_window);
        }
        void Maximize() override
        {
            SDL_MaximizeWindow(m_window);
        }
        void Minimize() override
        {
            SDL_MinimizeWindow(m_window);
        }
        void RestoreSize() override
        {
            SDL_RestoreWindow(m_window);
        }
        void MakeCurrent() override
        {
            SDL_GL_MakeCurrent(m_window, m_context);
        }
        void SwapBuffers() const override
        {
            SDL_GL_SwapWindow(m_window);
        }
        System::Position Position() const override
        {
            System::Position position;
            SDL_GetWindowPosition(m_window, &position.x, &position.y);
            return position;
        }
        System::Size Size() const override
        {
            System::Size size;
            SDL_GetWindowSize(m_window, &size.width, &size.height);
            return size;
        }

        System::Size DrawableSize() const override
        {
            System::Size size;
            SDL_GL_GetDrawableSize(m_window, &size.width, &size.height);
            return size;
        }

        SDL_Window* m_window = nullptr;
        void* m_context = nullptr;
    };

    Keycode KeycodeFromSDL(SDL_Scancode sdl_scancode)
    {
        switch(sdl_scancode)
        {
            case SDL_SCANCODE_0:
                return Keycode::ZERO;
            case SDL_SCANCODE_1:
                return Keycode::ONE;
            case SDL_SCANCODE_2:
                return Keycode::TWO;
            case SDL_SCANCODE_3:
                return Keycode::THREE;
            case SDL_SCANCODE_4:
                return Keycode::FOUR;
            case SDL_SCANCODE_5:
                return Keycode::FIVE;
            case SDL_SCANCODE_6:
                return Keycode::SIX;
            case SDL_SCANCODE_7:
                return Keycode::SEVEN;
            case SDL_SCANCODE_8:
                return Keycode::EIGHT;
            case SDL_SCANCODE_9:
                return Keycode::NINE;

            case SDL_SCANCODE_LEFT:
                return Keycode::LEFT;
            case SDL_SCANCODE_RIGHT:
                return Keycode::RIGHT;
            case SDL_SCANCODE_UP:
                return Keycode::UP;
            case SDL_SCANCODE_DOWN:
                return Keycode::DOWN;

            case SDL_SCANCODE_ESCAPE:
                return Keycode::ESCAPE;
            case SDL_SCANCODE_TAB:
                return Keycode::TAB;
            case SDL_SCANCODE_RETURN:
                return Keycode::ENTER;
            case SDL_SCANCODE_SPACE:
                return Keycode::SPACE;
            case SDL_SCANCODE_BACKSPACE:
                return Keycode::BACKSPACE;
            case SDL_SCANCODE_DELETE:
                return Keycode::DELETE;

            case SDL_SCANCODE_LCTRL:
                return Keycode::L_CTRL;
            case SDL_SCANCODE_RCTRL:
                return Keycode::R_CTRL;
            case SDL_SCANCODE_LSHIFT:
                return Keycode::L_SHIFT;
            case SDL_SCANCODE_RSHIFT:
                return Keycode::R_SHIFT;
            case SDL_SCANCODE_LALT:
                return Keycode::L_ALT;
            case SDL_SCANCODE_RALT:
                return Keycode::R_ALT;

            case SDL_SCANCODE_Q:
                return Keycode::Q;
            case SDL_SCANCODE_W:
                return Keycode::W;
            case SDL_SCANCODE_E:
                return Keycode::E;
            case SDL_SCANCODE_R:
                return Keycode::R;
            case SDL_SCANCODE_T:
                return Keycode::T;
            case SDL_SCANCODE_I:
                return Keycode::I;
            case SDL_SCANCODE_O:
                return Keycode::O;
            case SDL_SCANCODE_P:
                return Keycode::P;

            case SDL_SCANCODE_A:
                return Keycode::A;
            case SDL_SCANCODE_S:
                return Keycode::S;
            case SDL_SCANCODE_D:
                return Keycode::D;
            case SDL_SCANCODE_G:
                return Keycode::G;
            case SDL_SCANCODE_L:
                return Keycode::L;

            case SDL_SCANCODE_C:
                return Keycode::C;
            case SDL_SCANCODE_V:
                return Keycode::V;
            case SDL_SCANCODE_N:
                return Keycode::N;
            case SDL_SCANCODE_M:
                return Keycode::M;
            
            default:
                break;
        }

        return Keycode::NONE;
    }

    MouseButton MouseButtonFromSDL(Uint8 sdl_mouse_button)
    {
        switch(sdl_mouse_button)
        {
            case SDL_BUTTON_LEFT:
                return MouseButton::LEFT;
            case SDL_BUTTON_MIDDLE:
                return MouseButton::MIDDLE;
            case SDL_BUTTON_RIGHT:
                return MouseButton::RIGHT;
        }

        return MouseButton::NONE;
    }

    void HandleWindowEvent(const SDL_WindowEvent& event, System::IInputHandler* handler)
    {
        switch(event.event)
        {
            case SDL_WINDOWEVENT_SHOWN:
            case SDL_WINDOWEVENT_HIDDEN:
            case SDL_WINDOWEVENT_EXPOSED:
            case SDL_WINDOWEVENT_MOVED:
            case SDL_WINDOWEVENT_MINIMIZED:
            case SDL_WINDOWEVENT_MAXIMIZED:
            case SDL_WINDOWEVENT_RESTORED:
            case SDL_WINDOWEVENT_ENTER:
            case SDL_WINDOWEVENT_LEAVE:
            case SDL_WINDOWEVENT_CLOSE:
                break;
            case SDL_WINDOWEVENT_RESIZED:
                handler->OnSurfaceChanged(event.data1, event.data2);
                break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                handler->OnActivated(true);
                break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
                handler->OnActivated(false);
                break;
        }
    }

    void HandleUserEvent(const SDL_UserEvent& event, System::IInputHandler* handler)
    {
        if(event.code != TIMER_CALLBACK)
            return;

        // Function signature
        const System::timer_callback_t func = reinterpret_cast<System::timer_callback_t>(event.data1);
        func(event.data2);
    }

    class Timer : public System::ITimer
    {
    public:

        Timer(uint32_t ms, bool repeatingTimer, System::timer_callback_t callback, void* data)
            : m_ms(ms),
              m_repeatingTimer(repeatingTimer),
              m_callback(callback),
              m_data(data)
        {
            Start();
        }
        ~Timer()
        {
            Stop();
        }
        void Start() override
        {
            if(m_started)
                return;

            const auto callback_func = [] (Uint32 interval, void* data) -> Uint32 {
                const System::timer_callback_t sync_func = [](void* data) {
                    Timer* object = static_cast<Timer*>(data);
                    object->DoCallback();
                };

                SDL_Event event;
                event.type = SDL_USEREVENT;
                event.user.type = SDL_USEREVENT;
                event.user.code = TIMER_CALLBACK;
                event.user.data1 = reinterpret_cast<void*>(sync_func);
                event.user.data2 = data;
                SDL_PushEvent(&event);

                return interval;
            };

            m_timerId = SDL_AddTimer(m_ms, callback_func, this);
            m_started = true;
        }
        void Stop() override
        {
            if(m_started)
                SDL_RemoveTimer(m_timerId);
        }
        void DoCallback()
        {
            m_callback(m_data);
            if(!m_repeatingTimer)
                Stop();
        }

        const uint32_t m_ms;
        const bool m_repeatingTimer;
        const System::timer_callback_t m_callback;
        void* m_data;

        SDL_TimerID m_timerId = -1;
        bool m_started = false;
    };
}


void System::Initialize(const InitializeContext& context)
{
    std::memset(g_controller_states, 0, sizeof(g_controller_states));

    for(int index = 0; index < ControllerId::N_Controllers; ++index)
        g_controller_states[index].id = -1;

    if(context.log_file)
        g_log_file = std::fopen(context.log_file, "w");

    // Init SDL video subsystem
    const int result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC);
    if(result != 0)
        throw std::runtime_error("System|Couldn't initialize libSDL" + std::string(SDL_GetError()));

    SDL_version version;
    SDL_GetVersion(&version);

    Log("System");
    Log("\tSDL version: %u.%u.%u", version.major, version.minor, version.patch);

    char* base_path = SDL_GetBasePath();

    const char* working_directory = context.working_directory;
    if(working_directory == nullptr)
        working_directory = base_path;

#ifdef _WIN32
    const int chdir_result = _chdir(working_directory);
#else
    const int chdir_result = chdir(working_directory);
#endif
    if(chdir_result != 0)
        throw std::runtime_error("System|Unable to set resource directory");

    char cwd_buffer[1024] = { 0 };
#ifdef _WIN32
    _getcwd(cwd_buffer, std::size(cwd_buffer));
#else
    getcwd(cwd_buffer, std::size(cwd_buffer));
#endif
    Log("\tresouce directory: %s", cwd_buffer);

    SDL_free(base_path);
}

void System::Shutdown()
{
    if(g_log_file)
        std::fclose(g_log_file);

    SDL_Quit();
}

void System::Log(const char* fmt, ...)
{
    va_list va;
    va_start(va, fmt);

    if(g_log_file)
    {
        va_list file_log_args;
        va_copy(file_log_args, va);
        std::vfprintf(g_log_file, fmt, file_log_args);
        std::fflush(g_log_file);
        va_end(file_log_args);
    }

    //SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, fmt, va);
    std::vprintf(fmt, va);
    std::printf("\n");
    va_end(va);
}

uint32_t System::GetMilliseconds()
{
    return SDL_GetTicks();
}

uint64_t System::GetPerformanceCounter()
{
    return SDL_GetPerformanceCounter();
}

uint64_t System::GetPerformanceFrequency()
{
    return SDL_GetPerformanceFrequency();
}

System::ProcLoadFunc System::GetProcLoader()
{
    return SDL_GL_GetProcAddress;
}

void System::Sleep(uint32_t ms)
{
    SDL_Delay(ms);
}

System::ITimer* System::CreateTimer(uint32_t ms, bool one_shot, System::timer_callback_t callback, void* data)
{
    return new Timer(ms, !one_shot, callback, data);
}

System::Size System::GetCurrentWindowSize()
{
    SDL_DisplayMode mode;
    SDL_GetCurrentDisplayMode(0, &mode);

    return { mode.w, mode.h };
}

System::IWindow* System::MakeWindow(const char* title, int x, int y, int width, int height, WindowOptions options)
{
    return new SDLWindow(title, x, y, width, height, options);
}

void System::SetCursorVisibility(System::CursorVisibility state)
{
    const int sdl_state = (state == CursorVisibility::Shown) ? SDL_ENABLE : SDL_DISABLE;
    SDL_ShowCursor(sdl_state);
}

uint32_t System::GetClipboardText(char* output_buffer, uint32_t buffer_size)
{
    char* text = SDL_GetClipboardText();
    if(!text)
        return 0;
    
    const uint32_t length = std::strlen(text);
    const uint32_t n_chars = std::min(length, buffer_size);
    std::memcpy(output_buffer, text, n_chars);

    SDL_free(text);
    return n_chars;
}

void System::ProcessSystemEvents(System::IInputHandler* handler)
{
    const int modifier = SDL_GetModState();
    const bool ctrl  = (modifier & KMOD_CTRL);
    const bool shift = (modifier & KMOD_SHIFT);
    const bool alt   = (modifier & KMOD_ALT);
    const bool super = (modifier & KMOD_GUI);

    // Our SDL event placeholder.
    SDL_Event event;

    // Grab all the events off the queue.
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            // Keyboard
            case SDL_KEYDOWN:
            case SDL_KEYUP:
            {
                const Keycode keycode = KeycodeFromSDL(event.key.keysym.scancode);
                if(event.type == SDL_KEYDOWN)
                    handler->OnKeyDown(keycode, ctrl, shift, alt, super);
                else
                    handler->OnKeyUp(keycode, ctrl, shift, alt, super);
                break;
            }

            // Mouse
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            {
                const MouseButton mouse_button = MouseButtonFromSDL(event.button.button);
                if(event.type == SDL_MOUSEBUTTONDOWN)
                    handler->OnMouseDown(mouse_button, event.button.x, event.button.y, ctrl, shift, alt, super);
                else
                    handler->OnMouseUp(mouse_button, event.button.x, event.button.y, ctrl, shift, alt, super);
                break;
            }
            case SDL_MOUSEMOTION:
                handler->OnMouseMotion(event.motion.x, event.motion.y, ctrl, shift, alt, super);
                break;
            case SDL_MOUSEWHEEL:
                handler->OnMouseWheel(event.wheel.x, event.wheel.y, ctrl, shift, alt, super);
                break;

            // Touch
            case SDL_FINGERDOWN:
                handler->OnTouchDown(event.tfinger.fingerId, event.tfinger.x, event.tfinger.y, event.tfinger.dx, event.tfinger.dy);
                break;
            case SDL_FINGERUP:
                handler->OnTouchUp(event.tfinger.fingerId, event.tfinger.x, event.tfinger.y, event.tfinger.dx, event.tfinger.dy);
                break;
            case SDL_FINGERMOTION:
                handler->OnTouchMotion(event.tfinger.fingerId, event.tfinger.x, event.tfinger.y, event.tfinger.dx, event.tfinger.dy);
                break;
            case SDL_MULTIGESTURE:
                handler->OnMultiGesture(event.mgesture.x, event.mgesture.y, event.mgesture.dTheta, event.mgesture.dDist);
                break;

            // Controller
            case SDL_CONTROLLERDEVICEADDED:
            {
                int free_index = -1;

                for(int index = 0; index < ControllerId::N_Controllers; ++index)
                {
                    if(g_controller_states[index].id == -1)
                    {
                        free_index = index;
                        break;
                    }
                }

                if(free_index == -1)
                {
                    Log("System|Unable to find free controller index");
                    return;
                }

                const int id = event.cdevice.which;
                
                SDL_GameController* controller = SDL_GameControllerOpen(id);
                SDL_Joystick* joystick = SDL_GameControllerGetJoystick(controller);
                g_controller_states[free_index].id = SDL_JoystickInstanceID(joystick);

                Log("System|Controller connected (%s), index: %d, id: %d", SDL_GameControllerName(controller), free_index, g_controller_states[id].id);

                SDL_Haptic* haptic_device = SDL_HapticOpenFromJoystick(joystick);
                if(!haptic_device)
                    Log("System|Controller, unable to open haptic device. [%s]", SDL_GetError());

                const int result_code = SDL_HapticRumbleInit(haptic_device);
                if(result_code != 0)
                    Log("System[Controller, unable to initialize rumble. [%s]", SDL_GetError());

                g_controller_haptic[free_index] = haptic_device;

                handler->OnControllerAdded(free_index);
                break;
            }
            case SDL_CONTROLLERDEVICEREMOVED:
            {
                const int instance_id = event.cdevice.which;
                int controller_index = -1;

                for(int index = 0; index < ControllerId::N_Controllers; ++index)
                {
                    if(g_controller_states[index].id == instance_id)
                    {
                        controller_index = index;
                        break;
                    }
                }

                if(controller_index == -1)
                {
                    Log("System|Unable to find controller with id: %d", instance_id);
                    return;
                }

                handler->OnControllerRemoved(controller_index);
                g_controller_states[controller_index].id = -1;

                SDL_HapticClose(g_controller_haptic[controller_index]);
                g_controller_haptic[controller_index] = nullptr;

                SDL_GameController* controller = SDL_GameControllerFromInstanceID(instance_id);
                Log("System|Controller disconnected (%s), index: %d, id: %d",
                    SDL_GameControllerName(controller),
                    controller_index,
                    instance_id);
                SDL_GameControllerClose(controller);

                break;
            }

            // App
            case SDL_APP_TERMINATING:
                handler->OnAppTerminating();
                break;
            case SDL_APP_LOWMEMORY:
                break;
            case SDL_APP_WILLENTERBACKGROUND:
                handler->OnEnterBackground();
                break;
            case SDL_APP_DIDENTERBACKGROUND:
                break;
            case SDL_APP_WILLENTERFOREGROUND:
                break;
            case SDL_APP_DIDENTERFOREGROUND:
                handler->OnEnterForeground();
                break;

            // System
            case SDL_TEXTINPUT:
                handler->OnTextInput(event.text.text);
                break;
            case SDL_USEREVENT:
                HandleUserEvent(event.user, handler);
                break;
            case SDL_WINDOWEVENT:
                HandleWindowEvent(event.window, handler);
                break;
            case SDL_QUIT:
                handler->OnQuit();
                break;
        }
    }

    ProcessControllerState();
}

int System::KeycodeToNative(Keycode key)
{
    switch(key)
    {
        case Keycode::ZERO:
            return SDL_SCANCODE_0;
        case Keycode::ONE:
            return SDL_SCANCODE_1;
        case Keycode::TWO:
            return SDL_SCANCODE_2;
        case Keycode::THREE:
            return SDL_SCANCODE_3;
        case Keycode::FOUR:
            return SDL_SCANCODE_4;
        case Keycode::FIVE:
            return SDL_SCANCODE_5;
        case Keycode::SIX:
            return SDL_SCANCODE_6;
        case Keycode::SEVEN:
            return SDL_SCANCODE_7;
        case Keycode::EIGHT:
            return SDL_SCANCODE_8;
        case Keycode::NINE:
            return SDL_SCANCODE_9;

        case Keycode::LEFT:
            return SDL_SCANCODE_LEFT;
        case Keycode::RIGHT:
            return SDL_SCANCODE_RIGHT;
        case Keycode::UP:
            return SDL_SCANCODE_UP;
        case Keycode::DOWN:
            return SDL_SCANCODE_DOWN;

        case Keycode::ESCAPE:
            return SDL_SCANCODE_ESCAPE;
        case Keycode::TAB:
            return SDL_SCANCODE_TAB;
        case Keycode::ENTER:
            return SDL_SCANCODE_RETURN;
        case Keycode::SPACE:
            return SDL_SCANCODE_SPACE;
        case Keycode::BACKSPACE:
            return SDL_SCANCODE_BACKSPACE;
        case Keycode::DELETE:
            return SDL_SCANCODE_DELETE;

        case Keycode::L_CTRL:
            return SDL_SCANCODE_LCTRL;
        case Keycode::R_CTRL:
            return SDL_SCANCODE_RCTRL;
        case Keycode::L_SHIFT:
            return SDL_SCANCODE_LSHIFT;
        case Keycode::R_SHIFT:
            return SDL_SCANCODE_RSHIFT;
        case Keycode::L_ALT:
            return SDL_SCANCODE_LALT;
        case Keycode::R_ALT:
            return SDL_SCANCODE_RALT;

        case Keycode::Q:
            return SDL_SCANCODE_Q;
        case Keycode::W:
            return SDL_SCANCODE_W;
        case Keycode::E:
            return SDL_SCANCODE_E;
        case Keycode::R:
            return SDL_SCANCODE_R;
        case Keycode::T:
            return SDL_SCANCODE_T;
        case Keycode::I:
            return SDL_SCANCODE_I;
        case Keycode::O:
            return SDL_SCANCODE_O;
        case Keycode::P:
            return SDL_SCANCODE_P;

        case Keycode::A:
            return SDL_SCANCODE_A;
        case Keycode::S:
            return SDL_SCANCODE_S;
        case Keycode::D:
            return SDL_SCANCODE_D;
        case Keycode::G:
            return SDL_SCANCODE_G;
        case Keycode::L:
            return SDL_SCANCODE_L;

        case Keycode::C:
            return SDL_SCANCODE_C;
        case Keycode::V:
            return SDL_SCANCODE_V;
        case Keycode::N:
            return SDL_SCANCODE_N;
        case Keycode::M:
            return SDL_SCANCODE_M;

        default:
            break;
    }

    return SDL_SCANCODE_Q;
}

void System::ProcessControllerState()
{
    SDL_GameControllerUpdate();
    
    constexpr float dead_zone = 0.08f;
    const float max_value = float(std::numeric_limits<Sint16>::max());

    for(int index = 0; index < ControllerId::N_Controllers; ++index)
    {
        ControllerState& state = g_controller_states[index];
        if(state.id == -1)
            continue;

        state.button_state = 0;

        SDL_GameController* handle = SDL_GameControllerFromInstanceID(state.id);

        if(SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_A))
            state.button_state |= ControllerButton::A;
        if(SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_B))
            state.button_state |= ControllerButton::B;
        if(SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_X))
            state.button_state |= ControllerButton::X;
        if(SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_Y))
            state.button_state |= ControllerButton::Y;

        if(SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_LEFTSHOULDER))
            state.button_state |= ControllerButton::LEFT_SHOULDER;
        if(SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))
            state.button_state |= ControllerButton::RIGHT_SHOULDER;

        if(SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_LEFTSTICK))
            state.button_state |= ControllerButton::LEFT_STICK;
        if(SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_RIGHTSTICK))
            state.button_state |= ControllerButton::RIGHT_STICK;

        if(SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_BACK))
            state.button_state |= ControllerButton::BACK;
        if(SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_GUIDE))
            state.button_state |= ControllerButton::GUIDE;
        if(SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_START))
            state.button_state |= ControllerButton::START;

        if(SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_DPAD_UP))
            state.button_state |= ControllerButton::UP;
        if(SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_DPAD_DOWN))
            state.button_state |= ControllerButton::DOWN;
        if(SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_DPAD_LEFT))
            state.button_state |= ControllerButton::LEFT;
        if(SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
            state.button_state |= ControllerButton::RIGHT;

        state.left_x  = float(SDL_GameControllerGetAxis(handle, SDL_CONTROLLER_AXIS_LEFTX))  /  max_value;
        state.left_y  = float(SDL_GameControllerGetAxis(handle, SDL_CONTROLLER_AXIS_LEFTY))  / -max_value;
        state.right_x = float(SDL_GameControllerGetAxis(handle, SDL_CONTROLLER_AXIS_RIGHTX)) /  max_value;
        state.right_y = float(SDL_GameControllerGetAxis(handle, SDL_CONTROLLER_AXIS_RIGHTY)) / -max_value;

        state.left_trigger  = float(SDL_GameControllerGetAxis(handle, SDL_CONTROLLER_AXIS_TRIGGERLEFT)) /  max_value;
        state.right_trigger = float(SDL_GameControllerGetAxis(handle, SDL_CONTROLLER_AXIS_TRIGGERRIGHT)) / max_value;

        if(std::fabs(state.left_trigger) < dead_zone)
            state.left_trigger = 0.0f;

        if(std::fabs(state.right_trigger) < dead_zone)
            state.right_trigger = 0.0f;

        if(std::fabs(state.left_x) < dead_zone)
            state.left_x = 0.0f;

        if(std::fabs(state.left_y) < dead_zone)
            state.left_y = 0.0f;

        if(std::fabs(state.right_x) < dead_zone)
            state.right_x = 0.0f;

        if(std::fabs(state.right_y) < dead_zone)
            state.right_y = 0.0f;
    }
}

const System::ControllerState& System::GetController(ControllerId controller_id)
{
    const int id = static_cast<int>(controller_id);
    return g_controller_states[id];
}

bool System::IsControllerActive(ControllerId controller_id)
{
    const int id = static_cast<int>(controller_id);
    return g_controller_states[id].id != -1;
}

int System::GetControllerId(ControllerId controller_id)
{
    const int id = static_cast<int>(controller_id);
    return g_controller_states[id].id;
}

void System::PlayRumble(ControllerId controller_id, float strength, uint32_t duration_ms)
{
    SDL_Haptic* haptic = g_controller_haptic[static_cast<int>(controller_id)];
    if(haptic)
        SDL_HapticRumblePlay(haptic, strength, duration_ms);
}

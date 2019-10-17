
#include "System.h"
#include "Keycodes.h"
#include "open_gl.h"

#include "SDL.h"
#include "SDL_gamecontroller.h"
#include "SDL_keycode.h"
#include "SDL_log.h"

#include <stdexcept>
#include <cstdio>
#include <string>
#include <cmath>
#include <limits>


namespace
{
    constexpr int num_states = 2;
    System::ControllerState g_controller_states[num_states];

    enum PredefinedUserEventCode
    {
        TIMER_CALLBACK = 1
    };

    void PrintOpenGLInfo()
    {
        const GLubyte* vendor = glGetString(GL_VENDOR);
        const GLubyte* renderer = glGetString(GL_RENDERER);
        const GLubyte* version = glGetString(GL_VERSION);
        const GLubyte* glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION);
        //const GLubyte* extensions = glGetString(GL_EXTENSIONS);

        std::printf("OpenGL\n"
                    "\tvendor: %s\n"
                    "\trenderer: %s\n"
                    "\tversion: %s\n"
                    "\tglsl: %s\n", vendor, renderer, version, glslversion);
    }

    void SetupOpenGL()
    {
        glFrontFace(GL_CCW);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);

#ifndef __IPHONEOS__
        // To be able to use gl_PointSize in a vertex shader on regular
        // GLSL, we need to enable this thing, and its not avalible on
        // GLSL ES, that's why ifndef.
        glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
#endif
        glEnable(GL_POINT_SMOOTH);
        glEnable(GL_LINE_SMOOTH);

        // vsync on/off
        SDL_GL_SetSwapInterval(1);
    }

    class SDLWindow : public System::IWindow
    {
    public:

        SDLWindow(const char* title, int x, int y, int width, int height, bool fullscreen)
        {
            m_position.x = x;
            m_position.y = y;
            m_size.width = width;
            m_size.height = height;

            // Request opengl 2.1 context.
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
            SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
            SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
            SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

            //const unsigned int screenflag = fullscreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_RESIZABLE;
            const unsigned int flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE; // | SDL_WINDOW_BORDERLESS; //| SDL_WINDOW_ALLOW_HIGHDPI; // | screenflag;

            // Create our window centered and with the given resolution
            m_window = SDL_CreateWindow(title, x, y, width, height, flags);
            if(m_window == 0)
                throw std::runtime_error("System - Unable to create SDL window");

            m_context = SDL_GL_CreateContext(m_window);
            if(!m_context)
                throw std::runtime_error("System - Unable to create OpenGL context");
                        
            SetupOpenGL();
            MakeCurrent();
            PrintOpenGLInfo();
        }
        ~SDLWindow()
        {
            SDL_GL_DeleteContext(m_context);
            SDL_DestroyWindow(m_window);
        }
        void SurfaceChanged(int width, int height) override
        {
            m_size.width = width;
            m_size.height = height;
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
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glViewport(0, 0, m_size.width, m_size.height);
        }
        void SwapBuffers() const override
        {
            SDL_GL_SwapWindow(m_window);
        }
        void SetBackgroundColor(float red, float green, float blue) override
        {
            glClearColor(red, green, blue, 1.0f);
        }
        const System::Position& Position() const override
        {
            SDL_GetWindowPosition(m_window, &m_position.x, &m_position.y);
            return m_position;
        }
        const System::Size& Size() const override
        {
            return m_size;
        }

        mutable System::Position m_position;
        System::Size m_size;
        SDL_Window* m_window = nullptr;
        void* m_context = nullptr;
    };


    Keycode KeycodeFromSDL(SDL_Keycode sdl_key)
    {
        switch(sdl_key)
        {
            case SDLK_0:
                return Keycode::ZERO;
            case SDLK_1:
                return Keycode::ONE;
            case SDLK_2:
                return Keycode::TWO;
            case SDLK_3:
                return Keycode::THREE;
            case SDLK_4:
                return Keycode::FOUR;
            case SDLK_5:
                return Keycode::FIVE;
            case SDLK_6:
                return Keycode::SIX;
            case SDLK_7:
                return Keycode::SEVEN;
            case SDLK_8:
                return Keycode::EIGHT;
            case SDLK_9:
                return Keycode::NINE;

            case SDLK_LEFT:
                return Keycode::LEFT;
            case SDLK_RIGHT:
                return Keycode::RIGHT;
            case SDLK_UP:
                return Keycode::UP;
            case SDLK_DOWN:
                return Keycode::DOWN;

            case SDLK_ESCAPE:
                return Keycode::ESCAPE;
            case SDLK_TAB:
                return Keycode::TAB;
            case SDLK_RETURN:
                return Keycode::ENTER;
            case SDLK_SPACE:
                return Keycode::SPACE;
            case SDLK_BACKSPACE:
                return Keycode::BACKSPACE;
            case SDLK_DELETE:
                return Keycode::DELETE;

            case KMOD_LCTRL:
                return Keycode::L_CTRL;
            case KMOD_RCTRL:
                return Keycode::R_CTRL;
            case KMOD_LSHIFT:
                return Keycode::L_SHIFT;
            case KMOD_RSHIFT:
                return Keycode::R_SHIFT;
            case KMOD_LALT:
                return Keycode::L_ALT;
            case KMOD_RALT:
                return Keycode::R_ALT;

            case SDLK_q:
                return Keycode::Q;
            case SDLK_w:
                return Keycode::W;
            case SDLK_e:
                return Keycode::E;
            case SDLK_r:
                return Keycode::R;
            case SDLK_i:
                return Keycode::I;
            case SDLK_o:
                return Keycode::O;

            case SDLK_a:
                return Keycode::A;
            case SDLK_s:
                return Keycode::S;
            case SDLK_d:
                return Keycode::D;

            case SDLK_n:
                return Keycode::N;
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


void System::Initialize()
{
    // Init SDL video subsystem
    const int result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER);
    if(result != 0)
        throw std::runtime_error("System - Couldn't initialize libSDL" + std::string(SDL_GetError()));

    SDL_version version;
    SDL_GetVersion(&version);

    std::printf("System\n");
    std::printf("\tSDL version: %u.%u.%u\n", version.major, version.minor, version.patch);
}

void System::Shutdown()
{
    SDL_Quit();
}

void System::Log(const char* fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, fmt, va);
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

System::IWindow* System::CreateWindow(const char* title, int x, int y, int width, int height, bool fullscreen)
{
    return new SDLWindow(title, x, y, width, height, fullscreen);
}

void System::SetCursorVisibility(System::CursorVisibility state)
{
    const int sdl_state = (state == CursorVisibility::Shown) ? SDL_ENABLE : SDL_DISABLE;
    SDL_ShowCursor(sdl_state);
}

void System::ProcessSystemEvents(System::IInputHandler* handler)
{
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
                const Keycode keycode = KeycodeFromSDL(event.key.keysym.sym);

                const int modifier = SDL_GetModState();
                const bool ctrl  = (modifier & KMOD_LCTRL)  || (modifier & KMOD_RCTRL);
                const bool shift = (modifier & KMOD_LSHIFT) || (modifier & KMOD_RSHIFT);
                const bool alt   = (modifier & KMOD_LALT)   || (modifier & KMOD_RALT);

                if(event.type == SDL_KEYDOWN)
                    handler->OnKeyDown(keycode, ctrl, shift, alt);
                else
                    handler->OnKeyUp(keycode, ctrl, shift, alt);

                break;
            }

            // Mouse
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            {
                const MouseButton mouse_button = MouseButtonFromSDL(event.button.button);

                if(event.type == SDL_MOUSEBUTTONDOWN)
                    handler->OnMouseDown(mouse_button, event.button.x, event.button.y);
                else
                    handler->OnMouseUp(mouse_button, event.button.x, event.button.y);

                break;
            }
            case SDL_MOUSEMOTION:
                handler->OnMouseMotion(event.motion.x, event.motion.y);
                break;
            case SDL_MOUSEWHEEL:
                handler->OnMouseWheel(event.wheel.x, event.wheel.y);
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

                for(int index = 0; index < num_states; ++index)
                {
                    if(g_controller_states[index].id == -1)
                    {
                        free_index = index;        
                        break;
                    }
                }

                if(free_index == -1)
                {
                    std::printf("System - Unable to find free controller index\n");
                    return;
                }

                const int id = event.cdevice.which;
                
                SDL_GameController* controller = SDL_GameControllerOpen(id);
                SDL_Joystick* joystick = SDL_GameControllerGetJoystick(controller);

                g_controller_states[free_index].id = SDL_JoystickInstanceID(joystick);

                std::printf("System|Controller added: %d, %d, %s\n", free_index, g_controller_states[id].id, SDL_GameControllerName(controller));

                handler->OnControllerAdded(free_index);
                break;
            }
            case SDL_CONTROLLERDEVICEREMOVED:
            {
                const int instance_id = event.cdevice.which;
                int id = -1;

                for(int index = 0; index < num_states; ++index)
                {
                    if(g_controller_states[index].id == instance_id)
                    {
                        id = index;
                        break;
                    }
                }

                if(id == -1)
                {
                    std::printf("System - Unable to find controller with instance_id: %d\n", instance_id);                    
                    return;
                }

                handler->OnControllerRemoved(id);
                SDL_GameControllerClose(SDL_GameControllerFromInstanceID(instance_id));

                g_controller_states[id].id = -1;

                std::printf("System - Controller removed: %d, %d\n", id, instance_id);

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
            return SDLK_0;
        case Keycode::ONE:
            return SDLK_1;
        case Keycode::TWO:
            return SDLK_2;
        case Keycode::THREE:
            return SDLK_3;
        case Keycode::FOUR:
            return SDLK_4;
        case Keycode::FIVE:
            return SDLK_5;
        case Keycode::SIX:
            return SDLK_6;
        case Keycode::SEVEN:
            return SDLK_7;
        case Keycode::EIGHT:
            return SDLK_8;
        case Keycode::NINE:
            return SDLK_9;

        case Keycode::LEFT:
            return SDLK_LEFT;
        case Keycode::RIGHT:
            return SDLK_RIGHT;
        case Keycode::UP:
            return SDLK_UP;
        case Keycode::DOWN:
            return SDLK_DOWN;

        case Keycode::ESCAPE:
            return SDLK_ESCAPE;
        case Keycode::TAB:
            return SDLK_TAB;
        case Keycode::ENTER:
            return SDLK_RETURN;
        case Keycode::SPACE:
            return SDLK_SPACE;
        case Keycode::BACKSPACE:
            return SDLK_BACKSPACE;
        case Keycode::DELETE:
            return SDLK_DELETE;

        case Keycode::L_CTRL:
            return KMOD_LCTRL;
        case Keycode::R_CTRL:
            return KMOD_RCTRL;
        case Keycode::L_SHIFT:
            return KMOD_LSHIFT;
        case Keycode::R_SHIFT:
            return KMOD_RSHIFT;
        case Keycode::L_ALT:
            return KMOD_LALT;
        case Keycode::R_ALT:
            return KMOD_RALT;

        case Keycode::Q:
            return SDLK_q;
        case Keycode::W:
            return SDLK_w;
        case Keycode::E:
            return SDLK_e;
        case Keycode::R:
            return SDLK_r;
        case Keycode::I:
            return SDLK_i;
        case Keycode::O:
            return SDLK_o;

        case Keycode::A:
            return SDLK_a;
        case Keycode::S:
            return SDLK_s;
        case Keycode::D:
            return SDLK_d;

        case Keycode::N:
            return SDLK_n;

        default:
            break;
    }

    return SDLK_q;
}

void System::ProcessControllerState()
{
    SDL_GameControllerUpdate();
    
    constexpr float dead_zone = 0.08f;
    const float max_value = float(std::numeric_limits<Sint16>::max());

    for(int index = 0; index < num_states; ++index)
    {
        ControllerState& state = g_controller_states[index];
        if(state.id == -1)
            continue;

        SDL_GameController* handle = SDL_GameControllerFromInstanceID(state.id);

        state.a = SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_A);
        state.b = SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_B);
        state.x = SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_X);
        state.y = SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_Y);

        state.left_shoulder  = SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
        state.right_shoulder = SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);        
        
        state.left_stick  = SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_LEFTSTICK);
        state.right_stick = SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_RIGHTSTICK);

        state.back  = SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_BACK);
        state.guide = SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_GUIDE);
        state.start = SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_START);

        state.up    = SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_DPAD_UP);
        state.down  = SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
        state.left  = SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
        state.right = SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);

        state.left_x  = float(SDL_GameControllerGetAxis(handle, SDL_CONTROLLER_AXIS_LEFTX))  /  max_value;
        state.left_y  = float(SDL_GameControllerGetAxis(handle, SDL_CONTROLLER_AXIS_LEFTY))  / -max_value;
        state.right_x = float(SDL_GameControllerGetAxis(handle, SDL_CONTROLLER_AXIS_RIGHTX)) /  max_value;
        state.right_y = float(SDL_GameControllerGetAxis(handle, SDL_CONTROLLER_AXIS_RIGHTY)) / -max_value;

        state.left_trigger  = float(SDL_GameControllerGetAxis(handle, SDL_CONTROLLER_AXIS_TRIGGERLEFT)) /  max_value;
        state.right_trigger = float(SDL_GameControllerGetAxis(handle, SDL_CONTROLLER_AXIS_TRIGGERRIGHT)) / max_value;        

        if(std::fabs(state.left_shoulder) < dead_zone)
            state.left_shoulder = 0.0f;

        if(std::fabs(state.right_shoulder) < dead_zone)
            state.right_shoulder = 0.0f;

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


#include "System.h"

#include "OpenGL.h"

#include "SDL.h"

#include <stdexcept>
#include <cstdio>
#include <string>

namespace
{
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

        SDLWindow(const char* title, int width, int height, bool fullscreen)
        {
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
            m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
            if(m_window == 0)
                throw std::runtime_error("Unable to create SDL window");

            m_context = SDL_GL_CreateContext(m_window);
            if(!m_context)
                throw std::runtime_error("Unable to create OpenGL context");
                        
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
        const System::Size& Size() const override
        {
            return m_size;
        }

        System::Size m_size;
        SDL_Window* m_window = nullptr;
        void* m_context = nullptr;
    };

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
        using callback = void (*)(void*);
        const callback func = reinterpret_cast<callback>(event.data1);
        func(event.data2);

        handler->OnUserEvent(event.code, event.data1, event.data2);
    }

    class Timer : public System::ITimer
    {
    public:

        Timer(unsigned int ms, bool repeatingTimer, System::timer_callback_t callback, void* data)
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

        const unsigned int m_ms;
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
    const int result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    if(result != 0)
        throw std::runtime_error("Couldn't initialize libSDL" + std::string(SDL_GetError()));

    SDL_version version;
    SDL_GetVersion(&version);

    std::printf("SDL\n");
    std::printf("\tversion: %u.%u.%u\n", version.major, version.minor, version.patch);
}

void System::Shutdown()
{
    SDL_Quit();
}

unsigned int System::GetMilliseconds()
{
    return SDL_GetTicks();
}

void System::Sleep(unsigned int ms)
{
    SDL_Delay(ms);
}

System::ITimer* System::CreateOneShotTimer(unsigned int ms, System::timer_callback_t callback, void* data)
{
    return new Timer(ms, false, callback, data);
}

System::ITimer* System::CreateRepeatingTimer(unsigned int ms, System::timer_callback_t callback, void* data)
{
    return new Timer(ms, true, callback, data);
}

System::Size System::GetCurrentWindowSize()
{
    SDL_DisplayMode mode;
    SDL_GetCurrentDisplayMode(0, &mode);

    return { mode.w, mode.h };
}

System::IWindow* System::CreateWindow(const char* title, int width, int height, bool fullscreen)
{
    return new SDLWindow(title, width, height, fullscreen);
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
            case SDL_KEYDOWN:
                handler->OnKeyDown(event.key.keysym.sym, SDL_GetModState());
                break;
            case SDL_KEYUP:
                handler->OnKeyUp(event.key.keysym.sym, SDL_GetModState());
                break;
            case SDL_TEXTINPUT:
                handler->OnTextInput(event.text.text);
                break;
            case SDL_MOUSEBUTTONDOWN:
                handler->OnMouseDown(event.button.button, event.button.x, event.button.y);
                break;
            case SDL_MOUSEBUTTONUP:
                handler->OnMouseUp(event.button.button, event.button.x, event.button.y);
                break;
            case SDL_MOUSEMOTION:
                handler->OnMouseMotion(event.motion.x, event.motion.y);
                break;
            case SDL_MOUSEWHEEL:
                handler->OnMouseWheel(event.wheel.x, event.wheel.y);
                break;
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
            case SDL_USEREVENT:
                HandleUserEvent(event.user, handler);
                break;
            case SDL_WINDOWEVENT:
                HandleWindowEvent(event.window, handler);
                break;
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
            case SDL_QUIT:
                handler->OnQuit();
                break;
        }
    }
}

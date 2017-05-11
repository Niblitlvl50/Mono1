
#pragma once

#include "MonoFwd.h"
#include "System/System.h"
#include <functional>

namespace mono
{
    using CoordinateFunc = std::function<void (float&, float&)>;

    class InputHandler : public System::IInputHandler
    {
    public:

        InputHandler(const CoordinateFunc& func, EventHandler& eventHandler);

    private:

        virtual void OnKeyDown(Key::Keycode key, bool ctrl, bool shift, bool alt);
        virtual void OnKeyUp(Key::Keycode key, bool ctrl, bool shift, bool alt);
        virtual void OnTextInput(const char* text);
        virtual void OnMouseDown(unsigned int button, int x, int y);
        virtual void OnMouseUp(unsigned int button, int x, int y);
        virtual void OnMouseMotion(int x, int y);
        virtual void OnMouseWheel(int x, int y);
        virtual void OnTouchDown(int64_t touchId, float x, float y, float dx, float dy);
        virtual void OnTouchUp(int64_t touchId, float x, float y, float dx, float dy);
        virtual void OnTouchMotion(int64_t touchId, float x, float y, float dx, float dy);
        virtual void OnMultiGesture(float x, float y, float theta, float distance);
        virtual void OnUserEvent(int code, void* data1, void* data2);

        virtual void OnAppTerminating();
        virtual void OnEnterBackground();
        virtual void OnEnterForeground();
        virtual void OnQuit();

        virtual void OnSurfaceChanged(int width, int height);
        virtual void OnActivated(bool gain);

        const CoordinateFunc mScreenToWorldFunc;
        EventHandler& mEventHandler;
    };
}

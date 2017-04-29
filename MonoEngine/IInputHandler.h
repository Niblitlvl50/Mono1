
#pragma once

#include <cstdint>

namespace mono
{
    class IInputHandler
    {
    public:

        virtual ~IInputHandler()
        { }

        virtual void OnKeyDown(unsigned int key, int modifier) = 0;
        virtual void OnKeyUp(unsigned int key, int modifier) = 0;
        virtual void OnTextInput(const char* text) = 0;
        virtual void OnMouseDown(unsigned int button, int x, int y) = 0;
        virtual void OnMouseUp(unsigned int button, int x, int y) = 0;
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
}

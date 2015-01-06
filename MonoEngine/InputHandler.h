/*
 *  InputHandler.h
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-08.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "MonoFwd.h"
#include "IInputHandler.h"
#include <functional>

namespace mono
{
    typedef std::function<void (float&, float&)> CoordinateFunc;

    class InputHandler : public IInputHandler
    {
    public:
        
        InputHandler(const CoordinateFunc& func, EventHandler& eventHandler);
        
    private:
        
        virtual void OnKeyDown(int key);
        virtual void OnKeyUp(int key);
        virtual void OnMouseDown(unsigned int button, int x, int y);
        virtual void OnMouseUp(unsigned int button, int x, int y);
        virtual void OnMouseMotion(int x, int y);
        virtual void OnMouseWheel(int x, int y);
        virtual void OnTouchDown(float x, float y, float dx, float dy);
        virtual void OnTouchUp(float x, float y, float dx, float dy);
        virtual void OnTouchMotion(float x, float y, float dx, float dy);
        virtual void OnMultiGesture(float x, float y, float theta, float distance);
        virtual void OnUserEvent(int code, void* data1, void* data2);
        virtual void OnQuit();

        virtual void OnSurfaceChanged(int width, int height);
        virtual void OnActivated(bool gain);
        
        const CoordinateFunc mScreenToWorldFunc;
        EventHandler& mEventHandler;
    };
}


/*
 *  InputHandler.h
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-08.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "IInputHandler.h"
#include <functional>

namespace mono
{
    typedef std::function<void (int&, int&)> CoordinateFunc;

    class InputHandler : public IInputHandler
    {
    public:
        
        InputHandler(const CoordinateFunc& func);
        
    private:
        
        virtual void OnKeyDown(unsigned int key);
        virtual void OnKeyUp(unsigned int key);
        virtual void OnMouseDown(unsigned int button, int x, int y);
        virtual void OnMouseUp(unsigned int button, int x, int y);
        virtual void OnMouseMotion(int x, int y);
        virtual void OnMouseWheel(int x, int y);
        virtual void OnMultiGesture(int x, int y, float theta, float distance);
        virtual void OnUserEvent(int code, void* data1, void* data2);
        virtual void OnQuit();

        virtual void OnSurfaceChanged(int width, int height);
        virtual void OnActivated(bool gain);
        
        const CoordinateFunc mScreenToWorldFunc;
    };

}


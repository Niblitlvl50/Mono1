/*
 *  InputHandler.cpp
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-08.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "InputHandler.h"
#include "EventHandler.h"
#include "QuitEvent.h"
#include "SurfaceChangedEvent.h"
#include "ActivatedEvent.h"
#include "KeyDownEvent.h"
#include "KeyUpEvent.h"
#include "MouseUpEvent.h"
#include "MouseMotionEvent.h"
#include "GameController.h"
#include "Vector2f.h"

#include <SDL_events.h>

using namespace mono;


InputHandler::InputHandler(const CoordinateFunc& func)
    : mScreenToWorldFunc(func)
{ }

void InputHandler::OnKeyDown(unsigned int key)
{
    if(key == SDLK_ESCAPE)
        return;

    const Event::KeyDownEvent event(key);
    EventHandler::DispatchEvent(event);
}

void InputHandler::OnKeyUp(unsigned int key)
{
    if(key == SDLK_ESCAPE)
    {
        SDL_Event event;
        event.type = SDL_QUIT;
        SDL_PushEvent(&event);
        return;
    }

    const Event::KeyUpEvent event(key);
    EventHandler::DispatchEvent(event);
}

void InputHandler::OnMouseDown(unsigned int button, int x, int y)
{ }

void InputHandler::OnMouseUp(unsigned int button, int x, int y)
{
    mScreenToWorldFunc(x, y);
    const Event::MouseUpEvent event(button, x, y);
    EventHandler::DispatchEvent(event);
}

void InputHandler::OnMouseMotion(int x, int y)
{
    mScreenToWorldFunc(x, y);
    const Event::MouseMotionEvent event(x, y);
    EventHandler::DispatchEvent(event);
}

void InputHandler::OnUserEvent(int code, void* data1, void* data2)
{ }

void InputHandler::OnQuit()
{
    const Event::QuitEvent event;
    EventHandler::DispatchEvent(event);
}
void InputHandler::OnSurfaceChanged(int width, int height)
{
    const Event::SurfaceChangedEvent event(width, height);
    EventHandler::DispatchEvent(event);
}
void InputHandler::OnActivated(bool gain)
{
    const Event::ActivatedEvent event(gain);
    EventHandler::DispatchEvent(event);
}



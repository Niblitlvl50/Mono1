//
//  SysEvents.cpp
//  Monolith1
//
//  Created by Niblit on 2012-01-07.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "SysEvents.h"
#include "IInputHandler.h"
//#include <SDL_events.h> 

#include "SDL.h"


namespace
{
    void HandleWindowEvent(const SDL_WindowEvent& event, mono::IInputHandlerPtr handler)
    {
        switch(event.event)
        {
            case SDL_WINDOWEVENT_SHOWN:
                break;
            case SDL_WINDOWEVENT_HIDDEN:
                break;
            case SDL_WINDOWEVENT_EXPOSED:
                break;
            case SDL_WINDOWEVENT_MOVED:
                break;
            case SDL_WINDOWEVENT_RESIZED:
                handler->OnSurfaceChanged(event.data1, event.data2);
                break;
            case SDL_WINDOWEVENT_MINIMIZED:
                break;
            case SDL_WINDOWEVENT_MAXIMIZED:
                break;
            case SDL_WINDOWEVENT_RESTORED:
                break;
            case SDL_WINDOWEVENT_ENTER:
                break;
            case SDL_WINDOWEVENT_LEAVE:
                break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                handler->OnActivated(true);
                break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
                handler->OnActivated(false);
                break;
            case SDL_WINDOWEVENT_CLOSE:
                break;
        }
    
    }
}


void Events::ProcessSystemEvents(mono::IInputHandlerPtr handler)
{
    /* Our SDL event placeholder. */
    SDL_Event event;
	
    /* Grab all the events off the queue. */
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_KEYDOWN:
                handler->OnKeyDown(event.key.keysym.sym);
                break;

            case SDL_KEYUP:
                handler->OnKeyUp(event.key.keysym.sym);
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
                handler->OnTouchDown(event.tfinger.x, event.tfinger.y, event.tfinger.dx, event.tfinger.dy);
                break;
            
            case SDL_FINGERUP:
                handler->OnTouchUp(event.tfinger.x, event.tfinger.y, event.tfinger.dx, event.tfinger.dy);
                break;
                
            case SDL_FINGERMOTION:
                handler->OnTouchMotion(event.tfinger.x, event.tfinger.y, event.tfinger.dx, event.tfinger.dy);
                break;
                
            case SDL_MULTIGESTURE:
                handler->OnMultiGesture(event.mgesture.x, event.mgesture.y, event.mgesture.dTheta, event.mgesture.dDist);
                break;

            case SDL_USEREVENT:
                handler->OnUserEvent(event.user.code, event.user.data1, event.user.data2);
                break;

            case SDL_WINDOWEVENT:
                HandleWindowEvent(event.window, handler);
                break;

            case SDL_QUIT:
                handler->OnQuit();
                break;
        }
    }

}


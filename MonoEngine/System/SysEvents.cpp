//
//  SysEvents.cpp
//  Monolith1
//
//  Created by Niblit on 2012-01-07.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "SysEvents.h"
#include "IInputHandler.h"
#include "SDL_events.h"

namespace
{
    void HandleWindowEvent(const SDL_WindowEvent& event, const mono::IInputHandlerPtr& handler)
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

    bool HandlePredefinedUserEvent(const SDL_UserEvent& event)
    {
        if(event.code == Events::TIMER_CALLBACK)
        {
            // Function signature
            typedef void (*callback)(void*);

            const callback func = reinterpret_cast<callback>(event.data1);
            func(event.data2);

            return true;
        }

        return false;
    }
}


void Events::ProcessSystemEvents(const mono::IInputHandlerPtr& handler)
{
    // Our SDL event placeholder.
    SDL_Event event;
	
    // Grab all the events off the queue.
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
            {
                const bool handled = HandlePredefinedUserEvent(event.user);
                if(!handled)
                    handler->OnUserEvent(event.user.code, event.user.data1, event.user.data2);

                break;
            }

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


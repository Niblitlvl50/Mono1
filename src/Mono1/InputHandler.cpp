/*
 *  InputHandler.cpp
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-08.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "InputHandler.h"
#include "SDL.h"
#include "EventHandler.h"
#include "QuitEvent.h"
#include "SurfaceChangedEvent.h"
#include "ActivatedEvent.h"

using namespace mono;


void InputHandler::OnKeyDown(unsigned int key)
{ }

void InputHandler::OnKeyUp(unsigned int key)
{
	if(key == SDLK_ESCAPE)
	{
		SDL_Event event;
		event.type = SDL_QUIT;
		SDL_PushEvent(&event);
	}
	
	if(key == SDLK_q)
	{
		mLua.ExecuteString("print 'Hello ffs'");
	}
	
}

void InputHandler::OnMouseDown(unsigned int button)
{ }

void InputHandler::OnMouseUp(unsigned int button)
{
	switch(button)
	{
		case SDL_BUTTON_LEFT:
		{
			break;
		}
			
		case SDL_BUTTON_RIGHT:
		{
			break;
		}
			
		default:
			break;
	}
	
}

void InputHandler::OnMouseMotion(unsigned int x, unsigned int y)
{ }
void InputHandler::OnUserEvent(int code, void* data1, void* data2)
{ }

void InputHandler::OnQuit()
{
    Event::QuitEvent event;
    EventHandler::DispatchEvent(event);
}
void InputHandler::OnSurfaceChanged(int width, int height)
{
    Event::SurfaceChangedEvent event(width, height);
    EventHandler::DispatchEvent(event);
}
void InputHandler::OnActivated(bool gain)
{
    Event::ActivatedEvent event(gain);
    EventHandler::DispatchEvent(event);
}



/*
 *  MonolithEngine.cpp
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-07.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Engine.h"
#include "InputHandler.h"

#include "IWindow.h"
#include "IZone.h"
#include "IRenderer.h"

#include "SysTime.h"
#include "SysEvents.h"

#include "EventHandler.h"
#include "QuitEvent.h"
#include "SurfaceChangedEvent.h"
#include "ActivatedEvent.h"

#include "OGLRenderer.h"
#include "GameController.h"

#include "Vector2f.h"


using namespace mono;
using namespace std::tr1::placeholders;


Engine::Engine(float hz, IWindowPtr window, ICameraPtr camera)
    : mHz(hz),
      mWindow(window),
      mCamera(camera),
      mQuit(false),
      mInputHandler(new InputHandler)
{
    const Math::Vector2f windowSize(window->GetWidth(), window->GetHeight());
    GameControllerInstance().SetWindowSize(windowSize);

    const Event::QuitEventFunc quitFunc = std::tr1::bind(&Engine::OnQuit, this, _1);
    mQuitToken = EventHandler::AddListener(quitFunc);

    const Event::SurfaceChangedEventFunc surfaceChangedFunc = std::tr1::bind(&Engine::OnSurfaceChanged, this, _1);
    mSurfaceChangedToken = EventHandler::AddListener(surfaceChangedFunc);
	
    const Event::ActivatedEventFunc activatedFunc = std::tr1::bind(&Engine::OnActivated, this, _1);
    mActivatedToken = EventHandler::AddListener(activatedFunc);
}

Engine::~Engine()
{
    EventHandler::RemoveListener(mQuitToken);
    EventHandler::RemoveListener(mSurfaceChangedToken);
    EventHandler::RemoveListener(mActivatedToken);
}

void Engine::Run()
{
    const unsigned int timePerUpdate = 1000 / mHz;
    unsigned int lastTime = Time::GetMilliseconds();
    	
    while(!mQuit)
    {
        const unsigned int beforeTime = Time::GetMilliseconds();
        const unsigned int delta = beforeTime - lastTime;
		
        Events::ProcessSystemEvents(mInputHandler);

        // Rename this, its not really a renderer... it just collects what needs to be updated and drawn.
        OGLRenderer renderer;

        // The current zone
        IZonePtr zone = mono::GameControllerInstance().GetCurrentZone();
        zone->Accept(renderer);

        // Update the stuff, and then render the frame.
        renderer.Update(delta);
        mWindow->DrawFrame(renderer);

        lastTime = beforeTime;

        const int sleepTime = timePerUpdate - (Time::GetMilliseconds() - beforeTime);
		
        // Sleep with the time left here
        if(sleepTime > 0)
            Time::Sleep(sleepTime);
    }
}

void Engine::OnQuit(const Event::QuitEvent&)
{
    mQuit = true;
}

void Engine::OnSurfaceChanged(const Event::SurfaceChangedEvent& event)
{
    mWindow->SurfaceChanged(event.width, event.height);

    const Math::Vector2f windowSize(mWindow->GetWidth(), mWindow->GetHeight());
    GameControllerInstance().SetWindowSize(windowSize);
}

void Engine::OnActivated(const Event::ActivatedEvent& event)
{
    mWindow->Activated(event.gain);
}


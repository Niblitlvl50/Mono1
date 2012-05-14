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
#include "ICamera.h"
#include "IZone.h"

#include "SysTime.h"
#include "SysEvents.h"

#include "EventHandler.h"
#include "QuitEvent.h"
#include "SurfaceChangedEvent.h"
#include "ActivatedEvent.h"

#include "Renderer.h"
#include "GameController.h"

#include "Vector2f.h"

#include <stdexcept>



using namespace mono;
using namespace std::tr1::placeholders;


Engine::Engine(float hz, IWindowPtr window, ICameraPtr camera, IZonePtr zone)
    : mQuit(false),
      mHz(hz),
      mWindow(window),
      mCamera(camera),
      mZone(zone),
      mInputHandler(new InputHandler(std::tr1::bind(&Engine::ScreenToWorld, this, _1, _2)))
{
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
    // Do i put this in a raii object so if there is an exception thrown 
    // IZone::OnUnload is still called?  
    mZone->OnLoad(mCamera);
    
    const unsigned int timePerUpdate = 1000 / mHz;
    unsigned int lastTime = Time::GetMilliseconds();
    	
    while(!mQuit)
    {
        const unsigned int beforeTime = Time::GetMilliseconds();
        const unsigned int delta = beforeTime - lastTime;
		
        Events::ProcessSystemEvents(mInputHandler);

        Renderer renderer(mCamera);
        mZone->Accept(renderer);

        // Update the stuff, and then render the frame.
        renderer.Update(delta);
        mWindow->DrawFrame(renderer);

        lastTime = beforeTime;

        const int sleepTime = timePerUpdate - (Time::GetMilliseconds() - beforeTime);
		
        // Sleep with the time left here
        if(sleepTime > 0)
            Time::Sleep(sleepTime);
    }
    
    mZone->OnUnload();
}

void Engine::ScreenToWorld(int& x, int& y) const
{
    const Math::Vector2f& windowSize = mWindow->Size();
    const Math::Vector2f& cameraSize = mCamera->Size();
    const Math::Vector2f& cameraPosition = mCamera->Position();
    
    const Math::Vector2f scale = Math::Vector2f(cameraSize.mX / windowSize.mX, cameraSize.mY / windowSize.mY);
    
    const float screenX = x;
    const float screenY = windowSize.mY - y;
    
    const float tempx = screenX * scale.mX;
    const float tempy = screenY * scale.mY;
    
    x = tempx + cameraPosition.mX;
    y = tempy + cameraPosition.mY;
}

void Engine::OnQuit(const Event::QuitEvent&)
{
    mQuit = true;
}

void Engine::OnSurfaceChanged(const Event::SurfaceChangedEvent& event)
{
    mWindow->SurfaceChanged(event.width, event.height);
}

void Engine::OnActivated(const Event::ActivatedEvent& event)
{
    mWindow->Activated(event.gain);
}


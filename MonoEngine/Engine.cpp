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
#include "Quad.h"

#include <stdexcept>
#include <sstream>



using namespace mono;
using namespace std::tr1::placeholders;


Engine::Engine(unsigned int hz, IWindowPtr window, ICameraPtr camera, IZonePtr zone)
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
    
    long frame = 0;
    	
    while(!mQuit)
    {
        const unsigned int beforeTime = Time::GetMilliseconds();
        const unsigned int delta = beforeTime - lastTime;
		
        ++frame;        
        
        Events::ProcessSystemEvents(mInputHandler);

        Renderer renderer(mCamera, mWindow);
        mZone->Accept(renderer);

        // Update the stuff, and then render the frame.
        renderer.Update(delta);
        
        std::stringstream stream;
        stream << "Frame: " << frame;
        Color color = {1.0f, 1.0f, 1.0f, 1.0f};
        renderer.DrawText(stream.str(), mCamera->GetViewport().mA, false, color);
        renderer.DrawFrame();
        
        lastTime = beforeTime;

        const int sleepTime = timePerUpdate - (Time::GetMilliseconds() - beforeTime);
		
        // Sleep with the time left here
        if(sleepTime > 0)
            Time::Sleep(sleepTime);
    }
    
    mZone->OnUnload();
}

// This function is a little bit out of place but i guess this is where it has to be for now.
void Engine::ScreenToWorld(int& x, int& y) const
{
    const Math::Vector2f& windowSize = mWindow->Size();
    const Math::Quad& viewport = mCamera->GetViewport();
    
    const Math::Vector2f scale = viewport.mB / windowSize;
        
    const float screenX = x;
    const float screenY = windowSize.mY - y;
    
    const float tempx = screenX * scale.mX;
    const float tempy = screenY * scale.mY;
    
    x = tempx + viewport.mA.mX;
    y = tempy + viewport.mA.mY;
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


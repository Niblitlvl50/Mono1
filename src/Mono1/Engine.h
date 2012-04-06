/*
 *  MonolithEngine.h
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-07.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MONO_ENGINE_H
#define MONO_ENGINE_H

#include "MonoFwd.h"
#include "EventFwd.h"
#include "EventToken.h"

namespace mono
{

	class Engine
	{
	public:
		Engine(float hz, IWindowPtr view, ICameraPtr camera);
        ~Engine();
		
		void Run();
		
	private:
		
		void OnQuit(const Event::QuitEvent& event);
        void OnSurfaceChanged(const Event::SurfaceChangedEvent& event);
        void OnActivated(const Event::ActivatedEvent& event);
		
		bool mQuit;
		float mHz;
		
		IWindowPtr mWindow;
        ICameraPtr mCamera;
		IInputHandlerPtr mInputHandler;
        
        EventToken<Event::QuitEvent> mQuitToken;
        EventToken<Event::SurfaceChangedEvent> mSurfaceChangedToken;
        EventToken<Event::ActivatedEvent> mActivatedToken;
	};

}


#endif

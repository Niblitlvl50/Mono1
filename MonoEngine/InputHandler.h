/*
 *  InputHandler.h
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-08.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MONO_INPUTHANDLER_H
#define MONO_INPUTHANDLER_H

#include "IInputHandler.h"
#include "LuaInterpreter.h"


namespace mono
{
	
	class InputHandler : public IInputHandler
	{		
		virtual void OnKeyDown(unsigned int key);
		virtual void OnKeyUp(unsigned int key);
		virtual void OnMouseDown(unsigned int button);
		virtual void OnMouseUp(unsigned int button);
		virtual void OnMouseMotion(unsigned int x, unsigned int y);
        
        virtual void OnUserEvent(int code, void* data1, void* data2);
        virtual void OnQuit();
        
        virtual void OnSurfaceChanged(int width, int height);
        virtual void OnActivated(bool gain);

		LuaInterpreter mLua;
	};

}



#endif


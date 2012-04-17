/*
 *  IInputHandler.h
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-08.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

namespace mono
{

	struct IInputHandler
	{
		virtual ~IInputHandler()
        { }
		
		virtual void OnKeyDown(unsigned int key) = 0;
		virtual void OnKeyUp(unsigned int key) = 0;
		virtual void OnMouseDown(unsigned int button, int x, int y) = 0;
		virtual void OnMouseUp(unsigned int button, int x, int y) = 0;
		virtual void OnMouseMotion(unsigned int x, unsigned int y) = 0;
        virtual void OnUserEvent(int code, void* data1, void* data2) = 0;
        virtual void OnQuit() = 0;
        
        virtual void OnSurfaceChanged(int width, int height) = 0;
        virtual void OnActivated(bool gain) = 0;
	};

}



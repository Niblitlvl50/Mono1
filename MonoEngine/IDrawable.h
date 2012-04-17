/*
 *  IDrawable.h
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-21.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

namespace mono
{
    
	struct IDrawable
	{
		virtual ~IDrawable()
		{ }
		
		virtual void Draw() const = 0;
	};

}



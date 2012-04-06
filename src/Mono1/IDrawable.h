/*
 *  IDrawable.h
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-21.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MONO_IDRAWABLE_H
#define MONO_IDRAWABLE_H

namespace mono
{
    
	struct IDrawable
	{
		virtual ~IDrawable()
		{ }
		
		virtual void Draw() const = 0;
	};

}


#endif


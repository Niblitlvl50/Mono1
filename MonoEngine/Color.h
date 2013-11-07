//
//  Color.h
//  King_worksample_OSX
//
//  Created by Niklas Damberg on 06/11/13.
//
//

#pragma once

namespace mono
{
    struct Color
    {
        Color()
            : red(1.0f),
              green(1.0f),
              blue(1.0f),
              alpha(1.0f)
        { }
        
        Color(float r, float g, float b, float a)
            : red(r),
              green(g),
              blue(b),
              alpha(a)
        { }
        
        float red;
        float green;
        float blue;
        float alpha;
    };
}

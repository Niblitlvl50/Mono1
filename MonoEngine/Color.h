//
//  Color.h
//  Worksample_2
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
        { }
        
        Color(float r, float g, float b, float a)
            : red(r),
              green(g),
              blue(b),
              alpha(a)
        { }
        
        float red = 1.0f;
        float green = 1.0f;
        float blue = 1.0f;
        float alpha = 1.0f;
    };
}

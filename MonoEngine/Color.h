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
    namespace Color
    {
        struct RGBA
        {
            constexpr RGBA()
            { }

            constexpr RGBA(float r, float g, float b)
                : red(r),
                  green(g),
                  blue(b),
                  alpha(1.0f)
            { }

            constexpr RGBA(float r, float g, float b, float a)
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

        struct HSL
        {
            constexpr HSL()
            { }

            constexpr HSL(float h, float s, float l)
                : hue(h),
                  saturation(s),
                  lightness(l)
            { }

            float hue = 0.0f;
            float saturation = 0.0f;
            float lightness = 0.0f;
        };

        mono::Color::RGBA ToRGBA(const mono::Color::HSL& hsl);
        mono::Color::HSL ToHSL(const mono::Color::RGBA& rgb);
    }
}

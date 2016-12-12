
#include "Color.h"
#include <algorithm>

mono::Color::RGBA mono::Color::ToRGBA(const mono::Color::HSL& hsl)
{
    mono::Color::RGBA rgb;

    if(hsl.saturation == 0.0)
    {
        rgb.red = hsl.lightness;
        rgb.green = hsl.lightness;
        rgb.blue = hsl.lightness;
    }
    else
    {
        const auto func = [](float p, float q, float t) -> float {

            if(t < 0.0f)
                t += 1.0f;
            else if(t > 1.0f)
                t -= 1.0f;

            if(t < 1.0f / 6.0f)
                return p + (q - p) * 6.0f * t;

            /*
            if(t < 1.0f / 2.0f)
                return q;
            */

            if(1.0 / 6.0 <= t && t < 0.5)
                return q;

            if(t >= 0.5f && t < 2.0f / 3.0f)
                return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;

            return p;
        };

        const float value1 = hsl.lightness * (1.0f + hsl.saturation);
        const float value2 = hsl.lightness + hsl.saturation - hsl.lightness * hsl.saturation;

        const float q = (hsl.lightness < 0.5f) ? value1 : value2;
        const float p = 2.0f * hsl.lightness - q;

        constexpr float oneThird = 1.0f / 3.0f;

        rgb.red   = func(p, q, hsl.hue + oneThird);
        rgb.green = func(p, q, hsl.hue);
        rgb.blue  = func(p, q, hsl.hue - oneThird);
    }

    return rgb;
}

mono::Color::HSL mono::Color::ToHSL(const mono::Color::RGBA& rgb)
{
    const float red = rgb.red;
    const float green = rgb.green;
    const float blue = rgb.blue;

    const float max = std::max(std::max(red, green), blue);
    const float min = std::min(std::min(red, green), blue);

    mono::Color::HSL hsl;

    hsl.lightness = (max + min) / 2.0f;

    if(min == max)
    {
        hsl.hue = 0.0f;
        hsl.saturation = 0.0f;
    }
    else
    {
        const float delta = max - min;
        hsl.saturation = (hsl.lightness > 0.5f) ? delta / (2.0f - max - min) : delta / (max + min);

        if(max == red)
        {
            hsl.hue = (green - blue) / delta + (green < blue ? 6.0f : 0.0f);
        }
        else if(max == blue)
        {
            hsl.hue = (blue - red) / delta + 2.0f;
        }
        else
        {
            hsl.hue = (red - green) / delta + 4.0f;
        }

        hsl.hue /= 6.0f;
    }

    return hsl;
}

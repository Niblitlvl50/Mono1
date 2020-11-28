
#pragma once

#include <cstdint>

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
            float saturation = 1.0f;
            float lightness = 0.5f;
        };

        //! Convert HSL to RGBA
        mono::Color::RGBA ToRGBA(const mono::Color::HSL& hsl, float alpha = 1.0f);

        //! Convert RGBA to HSL
        mono::Color::HSL ToHSL(const mono::Color::RGBA& rgb);

        mono::Color::RGBA Lerp(const mono::Color::RGBA& first, const mono::Color::RGBA& second, float t);
        mono::Color::RGBA LerpRGB(const mono::Color::RGBA& first, const mono::Color::RGBA& second, float t);

        uint32_t ToHex(const mono::Color::RGBA& color);
        mono::Color::RGBA ToRGBA(uint32_t hex_color);

        constexpr mono::Color::RGBA WHITE       = mono::Color::RGBA(1.0f, 1.0f, 1.0f);
        constexpr mono::Color::RGBA OFF_WHITE   = mono::Color::RGBA(0.9f, 0.9f, 0.9f);
        constexpr mono::Color::RGBA GRAY        = mono::Color::RGBA(0.5f, 0.5f, 0.5f);
        constexpr mono::Color::RGBA BLACK       = mono::Color::RGBA(0.0f, 0.0f, 0.0f);
        constexpr mono::Color::RGBA RED         = mono::Color::RGBA(1.0f, 0.0f, 0.0f);
        constexpr mono::Color::RGBA GREEN       = mono::Color::RGBA(0.0f, 1.0f, 0.0f);
        constexpr mono::Color::RGBA BLUE        = mono::Color::RGBA(0.0f, 0.0f, 1.0f);
        constexpr mono::Color::RGBA MAGENTA     = mono::Color::RGBA(1.0f, 0.0f, 1.0f);
        constexpr mono::Color::RGBA CYAN        = mono::Color::RGBA(0.0f, 1.0f, 1.0f);
    }
}

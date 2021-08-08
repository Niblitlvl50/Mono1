
#pragma once

#include "Math/MathFunctions.h"
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <initializer_list>

using byte = unsigned char;

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

        template <int T>
        struct Gradient
        {
            float t[T];
            mono::Color::RGBA color[T];
        };

        mono::Color::RGBA MakeFromBytes(byte red, byte green, byte blue, byte alpha = 255);

        //! Convert HSL to RGBA
        mono::Color::RGBA ToRGBA(const mono::Color::HSL& hsl, float alpha = 1.0f);

        //! Convert RGBA to HSL
        mono::Color::HSL ToHSL(const mono::Color::RGBA& rgb);

        mono::Color::RGBA Lerp(const mono::Color::RGBA& first, const mono::Color::RGBA& second, float t);
        mono::Color::RGBA LerpRGB(const mono::Color::RGBA& first, const mono::Color::RGBA& second, float t);

        uint32_t ToHex(const mono::Color::RGBA& color);
        mono::Color::RGBA ToRGBA(uint32_t hex_color);

        template <typename T>
        inline mono::Color::RGBA ColorFromGradient(const T& gradient, float t_value)
        {
            int found_index = -1;

            for(int index = 0; index < (int)std::size(gradient.t); ++index)
            {
                const float t_gradient = gradient.t[index];
                if(t_gradient > t_value)
                    break;

                found_index = index;
            }

            const int max_index = int(std::size(gradient.t)) -1;
            const int first_index = std::clamp(found_index, 0, max_index);
            const int second_index = std::clamp(found_index + 1, 0, max_index);
            if(first_index == second_index)
                return gradient.color[first_index];

            const float gradient_t = math::Scale01(t_value, gradient.t[first_index], gradient.t[second_index]);
            return Lerp(gradient.color[first_index], gradient.color[second_index], gradient_t);
        }

        template <int T>
        inline Gradient<T> MakeGradient(std::initializer_list<float> ts, std::initializer_list<Color::RGBA> colors)
        {
            Gradient<T> gradient;
            std::memcpy(gradient.t, ts.begin(), sizeof(float) * ts.size());
            std::memcpy(gradient.color, colors.begin(), sizeof(Color::RGBA) * colors.size());

            return gradient;
        }

        constexpr mono::Color::RGBA WHITE       = mono::Color::RGBA(1.0f, 1.0f, 1.0f);
        constexpr mono::Color::RGBA OFF_WHITE   = mono::Color::RGBA(0.9f, 0.9f, 0.9f);
        constexpr mono::Color::RGBA GRAY        = mono::Color::RGBA(0.5f, 0.5f, 0.5f);
        constexpr mono::Color::RGBA BLACK       = mono::Color::RGBA(0.0f, 0.0f, 0.0f);
        constexpr mono::Color::RGBA RED         = mono::Color::RGBA(1.0f, 0.0f, 0.0f);
        constexpr mono::Color::RGBA ORANGE      = mono::Color::RGBA(1.0f, 0.5f, 0.0f);
        constexpr mono::Color::RGBA GREEN       = mono::Color::RGBA(0.0f, 1.0f, 0.0f);
        constexpr mono::Color::RGBA BLUE        = mono::Color::RGBA(0.0f, 0.0f, 1.0f);
        constexpr mono::Color::RGBA MAGENTA     = mono::Color::RGBA(1.0f, 0.0f, 1.0f);
        constexpr mono::Color::RGBA CYAN        = mono::Color::RGBA(0.0f, 1.0f, 1.0f);
    }
}

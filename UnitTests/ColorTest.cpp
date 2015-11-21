//
//  ColorTest.cpp
//  MonoOSX
//
//  Created by Niklas Damberg on 21/09/15.
//
//

#include "Color.h"
#include <gtest/gtest.h>

TEST(Color, DISABLED_ConvertToHSLAndBack)
{
    constexpr mono::Color::RGBA green(0.0, 1.0, 0.0);

    const mono::Color::HSL& hsl = mono::Color::ToHSL(green);
    const mono::Color::RGBA& rgb = mono::Color::ToRGBA(hsl);

    EXPECT_FLOAT_EQ(green.red,   rgb.red);
    EXPECT_FLOAT_EQ(green.green, rgb.green);
    EXPECT_FLOAT_EQ(green.blue,  rgb.blue);
}

TEST(Color, RedToRGBA)
{
    constexpr mono::Color::HSL red(0.0, 1.0, 0.5);
    const mono::Color::RGBA& rgb = mono::Color::ToRGBA(red);

    EXPECT_NEAR(rgb.red,   1.0f, 1e-5);
    EXPECT_NEAR(rgb.green, 0.0f, 1e-5);
    EXPECT_NEAR(rgb.blue,  0.0f, 1e-5);
}

TEST(Color, PinkToRGBA)
{
    constexpr float value = 283.7f / 360.0f;
    constexpr mono::Color::HSL pink(value, 0.775f, 0.542f);

    const mono::Color::RGBA& rgb = mono::Color::ToRGBA(pink);

    EXPECT_NEAR(rgb.red,   0.704f, 1e-4);
    EXPECT_NEAR(rgb.green, 0.187f, 1e-4);
    EXPECT_NEAR(rgb.blue,  0.897f, 1e-4);
}

TEST(Color, RedToHSL)
{
    constexpr mono::Color::RGBA red(1.0, 0.0, 0.0);
    const mono::Color::HSL& hsl = mono::Color::ToHSL(red);

    EXPECT_FLOAT_EQ(hsl.hue, 0.0f);
    EXPECT_FLOAT_EQ(hsl.saturation, 1.0f);
    EXPECT_FLOAT_EQ(hsl.lightness, 0.5f);
}

TEST(Color, RoyalBlueToHSL)
{
    constexpr mono::Color::RGBA royalBlue(0.255f, 0.104f, 0.918f);
    const mono::Color::HSL& hsl = mono::Color::ToHSL(royalBlue);

    constexpr float degree = 251.1f / 360.0f;

    EXPECT_FLOAT_EQ(hsl.hue, degree);
    EXPECT_FLOAT_EQ(hsl.saturation, 0.832f);
    EXPECT_FLOAT_EQ(hsl.lightness, 0.511f);
}

TEST(Color, RoyalBlueToRGBA)
{
    constexpr float degree = 251.1f / 360.0f;
    constexpr mono::Color::HSL royalBlue(degree, 0.832f, 0.511f);
    const mono::Color::RGBA& rgb = mono::Color::ToRGBA(royalBlue);

    EXPECT_NEAR(rgb.red,   0.255f, 1e-3);
    EXPECT_NEAR(rgb.green, 0.104f, 1e-3);
    EXPECT_NEAR(rgb.blue,  0.918f, 1e-3);
}


#include "Rendering/Color.h"
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

TEST(Color, DISABLED_RoyalBlueToHSL)
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

TEST(Color, Gradient)
{
    mono::Color::Gradient<3> gradient;

    gradient.t[0] = 0.0f;
    gradient.color[0] = mono::Color::WHITE;

    gradient.t[1] = 0.5f;
    gradient.color[1] = mono::Color::RED;

    gradient.t[2] = 1.0f;
    gradient.color[2] = mono::Color::BLACK;

    const mono::Color::RGBA color_neg = mono::Color::ColorFromGradient(gradient, -1.0f);
    EXPECT_NEAR(color_neg.red,   mono::Color::WHITE.red,   1e-3);
    EXPECT_NEAR(color_neg.green, mono::Color::WHITE.green, 1e-3);
    EXPECT_NEAR(color_neg.blue,  mono::Color::WHITE.blue,  1e-3);
    EXPECT_NEAR(color_neg.alpha, mono::Color::WHITE.alpha, 1e-3);

    const mono::Color::RGBA color_0 = mono::Color::ColorFromGradient(gradient, 0.0f);
    EXPECT_NEAR(color_0.red,   mono::Color::WHITE.red,   1e-3);
    EXPECT_NEAR(color_0.green, mono::Color::WHITE.green, 1e-3);
    EXPECT_NEAR(color_0.blue,  mono::Color::WHITE.blue,  1e-3);
    EXPECT_NEAR(color_0.alpha, mono::Color::WHITE.alpha, 1e-3);

    const mono::Color::RGBA color_1 = mono::Color::ColorFromGradient(gradient, 0.25f);
    const mono::Color::RGBA lerped_white_red = mono::Color::Lerp(mono::Color::WHITE, mono::Color::RED, 0.5f);
    EXPECT_NEAR(color_1.red,   lerped_white_red.red,   1e-3);
    EXPECT_NEAR(color_1.green, lerped_white_red.green, 1e-3);
    EXPECT_NEAR(color_1.blue,  lerped_white_red.blue,  1e-3);
    EXPECT_NEAR(color_1.alpha, lerped_white_red.alpha, 1e-3);

    const mono::Color::RGBA color_2 = mono::Color::ColorFromGradient(gradient, 0.5f);
    EXPECT_NEAR(color_2.red,   mono::Color::RED.red,   1e-3);
    EXPECT_NEAR(color_2.green, mono::Color::RED.green, 1e-3);
    EXPECT_NEAR(color_2.blue,  mono::Color::RED.blue,  1e-3);
    EXPECT_NEAR(color_2.alpha, mono::Color::RED.alpha, 1e-3);

    const mono::Color::RGBA color_3 = mono::Color::ColorFromGradient(gradient, 0.75f);
    const mono::Color::RGBA lerped_red_black = mono::Color::Lerp(mono::Color::RED, mono::Color::BLACK, 0.5f);
    EXPECT_NEAR(color_3.red,   lerped_red_black.red,   1e-3);
    EXPECT_NEAR(color_3.green, lerped_red_black.green, 1e-3);
    EXPECT_NEAR(color_3.blue,  lerped_red_black.blue,  1e-3);
    EXPECT_NEAR(color_3.alpha, lerped_red_black.alpha, 1e-3);

    const mono::Color::RGBA color_4 = mono::Color::ColorFromGradient(gradient, 1.0f);
    //const mono::Color::RGBA lerped_red_black = mono::Color::Lerp(mono::Color::RED, mono::Color::BLACK, 0.5f);
    EXPECT_NEAR(color_4.red,   mono::Color::BLACK.red,   1e-3);
    EXPECT_NEAR(color_4.green, mono::Color::BLACK.green, 1e-3);
    EXPECT_NEAR(color_4.blue,  mono::Color::BLACK.blue,  1e-3);
    EXPECT_NEAR(color_4.alpha, mono::Color::BLACK.alpha, 1e-3);

    const mono::Color::RGBA color_5 = mono::Color::ColorFromGradient(gradient, 1.5f);
    //const mono::Color::RGBA lerped_red_black = mono::Color::Lerp(mono::Color::RED, mono::Color::BLACK, 0.5f);
    EXPECT_NEAR(color_5.red,   mono::Color::BLACK.red,   1e-3);
    EXPECT_NEAR(color_5.green, mono::Color::BLACK.green, 1e-3);
    EXPECT_NEAR(color_5.blue,  mono::Color::BLACK.blue,  1e-3);
    EXPECT_NEAR(color_5.alpha, mono::Color::BLACK.alpha, 1e-3);
}

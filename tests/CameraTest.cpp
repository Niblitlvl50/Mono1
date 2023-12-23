
#include "Camera/Camera.h"
#include <gtest/gtest.h>

TEST(Camera, Viewport)
{
    mono::Camera camera;
    camera.SetPosition(math::Vector(0.0f, 0.0f));
    camera.SetViewportSize(math::Vector(100.0f, 66.0f));

    const math::Quad viewport = camera.GetViewport();

    EXPECT_FLOAT_EQ(viewport.bottom_left.x, -50.0f);
    EXPECT_FLOAT_EQ(viewport.bottom_left.y, -33.0f);
    EXPECT_FLOAT_EQ(viewport.top_right.x, 50.0f);
    EXPECT_FLOAT_EQ(viewport.top_right.y, 33.0f);
}

TEST(Camera, ScreenToWorld)
{
    mono::Camera camera;
    camera.SetPosition(math::Vector(0.0f, 0.0f));
    camera.SetViewportSize(math::Vector(100.0f, 66.0f));
    camera.SetWindowSize(math::Vector(math::Vector(640.0f, 480.0f)));

    const math::Vector world_position = camera.ScreenToWorld(math::Vector(10.0f, 4.0f));

    EXPECT_FLOAT_EQ(world_position.x, -48.4375);
    EXPECT_FLOAT_EQ(world_position.y, 36.875);
}


#include <gtest/gtest.h>
#include "Rendering/Sprite/Serialize.h"

TEST(SpriteSerizlieTest, ReadSpriteFile)
{
    const nlohmann::json sprite_data_json = nlohmann::json::parse(R"(
        {
            "animations": [
                {
                    "frame_duration": 100,
                    "frames": [
                        0,
                        1
                    ],
                    "loop": true,
                    "name": "default"
                }
            ],
            "frames": [
                {
                    "h": 17,
                    "name": "ammo_icon_1",
                    "w": 7,
                    "x": 191,
                    "y": 486
                },
                {
                    "h": 12,
                    "name": "ammo_icon_2",
                    "w": 4,
                    "x": 391,
                    "y": 586
                }
            ],
            "frames_offsets": [
                {
                    "x": 0.0,
                    "y": 0.0
                },
                {
                    "x": 1.0,
                    "y": 1.0
                }
            ],
            "source_folder": "res/images/icons/",
            "texture": "res/sprite_atlas.png",
            "texture_size": {
                "h": 1024,
                "w": 2048
            }
        }
    )");

    const mono::SpriteData sprite_data = sprite_data_json;

    ASSERT_EQ(sprite_data.texture_file, "res/sprite_atlas.png");
    ASSERT_EQ(sprite_data.source_folder, "res/images/icons/");
    ASSERT_FLOAT_EQ(sprite_data.texture_size.x, 2048.0f);
    ASSERT_FLOAT_EQ(sprite_data.texture_size.y, 1024.0f);

    ASSERT_EQ(sprite_data.frames.size(), 2ul);
    ASSERT_EQ(sprite_data.animations.size(), 1ul);

    {
        const mono::SpriteFrame& frame1 = sprite_data.frames[0];

        ASSERT_EQ(frame1.size.x, 7.0f);
        ASSERT_EQ(frame1.size.y, 17.0f);
    
        ASSERT_EQ(frame1.uv_upper_left.x * sprite_data.texture_size.x, 191.0f);
        ASSERT_EQ(frame1.uv_upper_left.y * sprite_data.texture_size.y, 486.0f + 17.0f);
        ASSERT_EQ(frame1.uv_lower_right.x * sprite_data.texture_size.x, 191.0f + 7.0f);
        ASSERT_EQ(frame1.uv_lower_right.y * sprite_data.texture_size.y, 486.0f);
    
        ASSERT_EQ(frame1.center_offset.x, 0.0f);
        ASSERT_EQ(frame1.center_offset.y, 0.0f);
    }

    {
        const mono::SpriteFrame& frame2 = sprite_data.frames[1];

        ASSERT_EQ(frame2.size.x, 4.0f);
        ASSERT_EQ(frame2.size.y, 12.0f);
    
        ASSERT_EQ(frame2.uv_upper_left.x * sprite_data.texture_size.x, 391.0f);
        ASSERT_EQ(frame2.uv_upper_left.y * sprite_data.texture_size.y, 586.0f + 12.0f);
        ASSERT_EQ(frame2.uv_lower_right.x * sprite_data.texture_size.x, 391.0f + 4.0f);
        ASSERT_EQ(frame2.uv_lower_right.y * sprite_data.texture_size.y, 586.0f);
    
        ASSERT_EQ(frame2.center_offset.x, 1.0f);
        ASSERT_EQ(frame2.center_offset.y, 1.0f);
    }

    {
        const mono::SpriteAnimation& animation1 = sprite_data.animations[0];
        ASSERT_EQ(animation1.name, "default");
        ASSERT_TRUE(animation1.looping);
        ASSERT_EQ(animation1.frame_duration, 100);
        ASSERT_EQ(animation1.frames.size(), 2ul);

        {
            const mono::SpriteAnimationFrame& frame1 = animation1.frames[0];
            ASSERT_EQ(frame1.frame, 0);
            ASSERT_EQ(frame1.notify, "");
        }

        {
            const mono::SpriteAnimationFrame& frame2 = animation1.frames[1];
            ASSERT_EQ(frame2.frame, 1);
            ASSERT_EQ(frame2.notify, "");
        }
    }
}

TEST(SpriteSerizlieTest, WriteSpriteData)
{
    mono::SpriteData sprite_data_source;
    sprite_data_source.hash = 0;
    sprite_data_source.texture_file = "res/sprite_sheet.png";
    sprite_data_source.source_folder = "res/images/test";
    sprite_data_source.texture_size = math::Vector(256.0f, 512.0f);

    mono::SpriteFrame frame1;
    frame1.size = math::Vector(7.0f, 17.0f);
    frame1.uv_upper_left = math::Vector(191.0f, 486.0f + 17.0f);
    frame1.uv_lower_right = math::Vector(191.0f + 7.0f, 486.0f);
    frame1.center_offset = math::Vector(0.0f, 0.0f);

    mono::SpriteFrame frame2;
    frame2.size = math::Vector(4.0f, 12.0f);
    frame2.uv_upper_left = math::Vector(391.0f, 586.0f + 12.0f);
    frame2.uv_lower_right = math::Vector(391.0f + 4.0f, 586.0f);
    frame2.center_offset = math::Vector(1.0f, 1.0f);

    sprite_data_source.frames = { frame1, frame2 };

    mono::SpriteAnimation animation1;
    animation1.name = "default";
    animation1.looping = true;
    animation1.frame_duration = 100;
    animation1.frames = {
        { 0, ""},
        { 1, " "}
    };
    sprite_data_source.animations = { animation1 };

    const nlohmann::json sprite_data_json = sprite_data_source;

    {
        const mono::SpriteData& sprite_data_deserialized = sprite_data_json;
        
        ASSERT_EQ(sprite_data_deserialized.texture_file, sprite_data_source.texture_file);
        ASSERT_EQ(sprite_data_deserialized.source_folder, sprite_data_source.source_folder);
        ASSERT_FLOAT_EQ(sprite_data_deserialized.texture_size.x, sprite_data_source.texture_size.x);
        ASSERT_FLOAT_EQ(sprite_data_deserialized.texture_size.y, sprite_data_source.texture_size.y);

        ASSERT_EQ(sprite_data_deserialized.frames.size(), sprite_data_source.frames.size());
        ASSERT_EQ(sprite_data_deserialized.animations.size(), sprite_data_source.animations.size());

        {
            const mono::SpriteFrame& frame1 = sprite_data_deserialized.frames[0];

            ASSERT_EQ(frame1.size.x, 7.0f);
            ASSERT_EQ(frame1.size.y, 17.0f);
        
            ASSERT_EQ(frame1.uv_upper_left.x * sprite_data_deserialized.texture_size.x, 191.0f);
            ASSERT_EQ(frame1.uv_upper_left.y * sprite_data_deserialized.texture_size.y, 486.0f + 17.0f);
            ASSERT_EQ(frame1.uv_lower_right.x * sprite_data_deserialized.texture_size.x, 191.0f + 7.0f);
            ASSERT_EQ(frame1.uv_lower_right.y * sprite_data_deserialized.texture_size.y, 486.0f);
        
            ASSERT_EQ(frame1.center_offset.x, 0.0f);
            ASSERT_EQ(frame1.center_offset.y, 0.0f);
        }

        {
            const mono::SpriteFrame& frame2 = sprite_data_deserialized.frames[1];

            ASSERT_EQ(frame2.size.x, 4.0f);
            ASSERT_EQ(frame2.size.y, 12.0f);
        
            ASSERT_EQ(frame2.uv_upper_left.x * sprite_data_deserialized.texture_size.x, 391.0f);
            ASSERT_EQ(frame2.uv_upper_left.y * sprite_data_deserialized.texture_size.y, 586.0f + 12.0f);
            ASSERT_EQ(frame2.uv_lower_right.x * sprite_data_deserialized.texture_size.x, 391.0f + 4.0f);
            ASSERT_EQ(frame2.uv_lower_right.y * sprite_data_deserialized.texture_size.y, 586.0f);
        
            ASSERT_EQ(frame2.center_offset.x, 1.0f);
            ASSERT_EQ(frame2.center_offset.y, 1.0f);
        }

        {
            const mono::SpriteAnimation& animation1 = sprite_data_deserialized.animations[0];
            ASSERT_EQ(animation1.name, "default");
            ASSERT_TRUE(animation1.looping);
            ASSERT_EQ(animation1.frame_duration, 100);
            ASSERT_EQ(animation1.frames.size(), 2ul);

            {
                const mono::SpriteAnimationFrame& frame1 = animation1.frames[0];
                ASSERT_EQ(frame1.frame, 0);
                ASSERT_EQ(frame1.notify, "");
            }

            {
                const mono::SpriteAnimationFrame& frame2 = animation1.frames[1];
                ASSERT_EQ(frame2.frame, 1);
                ASSERT_EQ(frame2.notify, "");
            }
        }
    }
}

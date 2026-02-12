
#pragma once

#include "Rendering/Sprite/SpriteData.h"
#include "Math/Serialize.h"
#include "nlohmann/json.hpp"

namespace mono
{
    inline void to_json(nlohmann::json& json, const mono::SpriteData& sprite_data)
    {
        json["texture"] = sprite_data.texture_file;
        json["source_folder"] = sprite_data.source_folder;

        nlohmann::json texture_size_json;
        texture_size_json["w"] = (int)sprite_data.texture_size.x;
        texture_size_json["h"] = (int)sprite_data.texture_size.y;
        json["texture_size"] = texture_size_json;

        // Frames
        {
            nlohmann::json& frames_json = json["frames"];
            for(size_t index = 0; index < sprite_data.frames.size(); ++index)
            {
                const mono::SpriteFrame& frame = sprite_data.frames[index];

                nlohmann::json frame_json;
                frame_json["name"] = "unknown";
                frame_json["w"] = (int)frame.size.x;
                frame_json["h"] = (int)frame.size.y;
                frame_json["x"] = (int)frame.uv_upper_left.x;
                frame_json["y"] = (int)frame.uv_lower_right.y;

                frames_json.push_back(frame_json);
            }
        }

        // Frame Offsets
        {
            nlohmann::json& frame_offsets_json = json["frames_offsets"];

            for(size_t index = 0; index < sprite_data.frames.size(); ++index)
            {
                const mono::SpriteFrame& frame = sprite_data.frames[index];
                frame_offsets_json[index]["x"] = frame.center_offset.x;
                frame_offsets_json[index]["y"] = frame.center_offset.y;
            }
        }

        // Animations
        {
            nlohmann::json& animations_json = json["animations"];

            for(const mono::SpriteAnimation& animation : sprite_data.animations)
            {
                std::vector<int> frames;
                std::vector<std::string> notifies;
                
                for(const mono::SpriteAnimationFrame& animation_frame : animation.frames)
                {
                    frames.push_back(animation_frame.frame);
                    notifies.push_back(animation_frame.notify);
                } 
                
                nlohmann::json object;
                object["name"] = animation.name;
                object["loop"] = animation.looping;
                object["frame_duration"] = animation.frame_duration;
                
                object["frames"] = frames;
                object["notifies"] = notifies;
                
                animations_json.push_back(object);
            }
        }
    }

    inline void from_json(const nlohmann::json& json, mono::SpriteData& sprite_data)
    {
        const nlohmann::json& texture_size = json["texture_size"];
        const nlohmann::json& frames = json["frames"];
        const nlohmann::json& frames_offsets = json["frames_offsets"];

        //sprite_data.hash = sprite_hash;
        sprite_data.texture_file = json["texture"];
        sprite_data.source_folder = json.value("source_folder", "");
        sprite_data.texture_size = math::Vector(texture_size["w"], texture_size["h"]);

        sprite_data.frames.reserve(frames.size());

        for(size_t index = 0; index < frames.size(); ++index)
        {
            const nlohmann::json& frame = frames[index];

            const float x = float(frame["x"]);
            const float y = float(frame["y"]);
            const float w = float(frame["w"]);
            const float h = float(frame["h"]);

            const nlohmann::json& frame_offset = frames_offsets[index];

            mono::SpriteFrame sprite_frame;
            sprite_frame.center_offset.x = frame_offset["x"];
            sprite_frame.center_offset.y = frame_offset["y"];
            sprite_frame.uv_upper_left = math::Vector(x, y + h);
            sprite_frame.uv_lower_right = math::Vector(x + w, y);

            const float width = (sprite_frame.uv_lower_right.x - sprite_frame.uv_upper_left.x);
            const float height = (sprite_frame.uv_upper_left.y - sprite_frame.uv_lower_right.y);

            sprite_frame.size = math::Vector(width, height);
            sprite_data.frames.push_back(sprite_frame);
        }

        const nlohmann::json& animations = json["animations"];
        sprite_data.animations.reserve(animations.size());

        for(const nlohmann::json& animation_json : animations)
        {
            mono::SpriteAnimation sprite_animation;
            sprite_animation.name = animation_json["name"];
            sprite_animation.looping = animation_json["loop"];
            sprite_animation.frame_duration = animation_json["frame_duration"];

            const std::vector<int> frames = animation_json["frames"].get<std::vector<int>>();
            const std::vector<std::string> frame_notifies = animation_json.value("notifies", std::vector<std::string>(frames.size()));

            for(size_t index = 0; index < frames.size(); ++index)
            {
                mono::SpriteAnimationFrame animation_frame;
                animation_frame.frame = frames[index];
                animation_frame.notify = frame_notifies[index];

                sprite_animation.frames.push_back(animation_frame);
            }

            sprite_data.animations.push_back(sprite_animation);
        }
    }
}


#pragma once

#include "Rendering/Sprite/AnimationSequence.h"
#include "Math/Quad.h"
#include <vector>
#include <functional>

namespace animator
{
    struct UIContext
    {
        int tools_texture_id;
        math::Quad save_icon;
        math::Quad add_icon;
        math::Quad plus_icon;
        math::Quad delete_icon;

        int animation_id;
        int animations;
        const char* display_name;
        bool loop_animation;
        int max_frame_id;
        int active_frame;
        std::vector<mono::Frame>* frames;

        std::function<void (bool)> on_loop_toggle;
        std::function<void ()> on_add_animation;
        std::function<void ()> on_delete_animation;
        std::function<void ()> on_add_frame;
        std::function<void (int)> on_delete_frame;
        std::function<void (const char*)> on_name_animation;

        std::function<void ()> on_save;
    };
}

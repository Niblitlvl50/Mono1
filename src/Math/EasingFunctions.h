
#pragma once

namespace math
{
    //
    // http://gizma.com/easing/
    // A bunch of easing functions for movement.
    //
    // b: start value
    // c: change in value
    //

    inline float LinearTween(float time, float duration, float start_value, float delta_value)
    {
    	return delta_value * time / duration + start_value;
    };

    inline float EaseInCubic(float time, float duration, float start_value, float delta_value)
    {
        time /= duration;
        return delta_value * time * time * time + start_value;
    };
    
    inline float EaseOutCubic(float time, float duration, float start_value, float delta_value)
    {
        time /= duration;
        time -= 1.0f;
        return delta_value * (time * time * time + 1.0f) + start_value;
    };

    inline float EaseInOutCubic(float time, float duration, float start_value, float delta_value)
    {
        time /= duration / 2.0f;
        if(time < 1.0f)
            return delta_value / 2.0f * time * time * time + start_value;
        
        time -= 2.0f;
        return delta_value / 2.0f * (time * time * time + 2.0f) + start_value;
    };

    inline float EaseBackIn(float time, float duration, float start_value, float delta_value)
    {
        time /= duration;
        constexpr float s = 1.70158f;
        return delta_value * time * time * ((s + 1.0f) * time - s) + start_value;
    }

    inline float EaseBackOut(float time, float duration, float start_value, float delta_value)
    {
        constexpr float s = 1.70158f;
        const float t0 = time / duration - 1.0f;
        return delta_value * (t0 * t0 * ((s + 1.0f) * t0 + s) + 1.0f) + start_value;
    }

    inline float EaseBackInOut(float time, float duration, float start_value, float delta_value)
    {
        constexpr float s = 1.70158f;
        const float s0 = s * 1.525f;
        float t0 = time / (duration / 2.0f);
        if (t0 < 1.0f)
            return delta_value / 2.0f * (t0 * t0 * ((s0 + 1.0f) * t0 - s0)) + start_value;
        
        t0 -= 2.0f;
        return delta_value / 2.0f * (t0 * t0 * ((s0 + 1.0f) * t0 + s0) + 2.0f) + start_value;
    }

    using EaseFunction = float (*)(float time, float duration, float start_value, float delta_value);

    constexpr EaseFunction ease_functions[] = {
        LinearTween,
        EaseInCubic,
        EaseOutCubic,
        EaseInOutCubic,
        EaseBackIn,
        EaseBackOut,
        EaseBackInOut,
    };

    enum class EasingFuncType
    {
        LINEAR_TWEEN,
        EASE_IN_CUBIC,
        EASE_OUT_CUBIC,
        EASE_IN_OUT_CUBIC,
        EASE_BACK_IN,
        EASE_BACK_OUT,
        EASE_BACK_IN_OUT,
    };

    constexpr const char* easing_function_strings[] = {
        "Linear Tween",
        "Ease In Cubic",
        "Ease Out Cubic",
        "Ease In/Out Cubic",
        "Ease Back In",
        "Ease Back Out",
        "Ease Back In Out",
    };

    inline const char* EasingFuncTypeToString(EasingFuncType logic_type)
    {
        return easing_function_strings[static_cast<int>(logic_type)];
    }
}

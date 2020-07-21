
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

    using EaseFunction = float (*)(float time, float duration, float start_value, float delta_value);
}

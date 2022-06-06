
//
// https://theorangeduck.com/page/spring-roll-call#critical
//

#pragma once

#include "Math/Vector.h"

namespace math
{
    inline float halflife_to_damping(float halflife, float eps = 1e-5f)
    {
        return (4.0f * 0.69314718056f) / (halflife + eps);
    }
        
    inline float damping_to_halflife(float damping, float eps = 1e-5f)
    {
        return (4.0f * 0.69314718056f) / (damping + eps);
    }

    inline float fast_negexp(float x)
    {
        return 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);
    }

    inline void simple_spring_damper_implicit(float& x, float& v, float x_goal, float halflife, float dt)
    {
        float y = halflife_to_damping(halflife) / 2.0f;	
        float j0 = x - x_goal;
        float j1 = v + j0 * y;
        float eydt = fast_negexp(y * dt);

        x = eydt * (j0 + j1 * dt) + x_goal;
        v = eydt * (v - j1 * y * dt);
    }

    inline void critical_spring_damper_implicit(float& x, float& v, float x_goal, float v_goal, float halflife, float dt)
    {
        float g = x_goal;
        float q = v_goal;
        float d = halflife_to_damping(halflife);
        float c = g + (d * q) / ((d * d) / 4.0f);
        float y = d / 2.0f;
        float j0 = x - c;
        float j1 = v + j0 * y;
        float eydt = fast_negexp(y * dt);

        x = eydt * (j0 + j1 * dt) + c;
        v = eydt * (v - j1 * y * dt);
    }

    inline void critical_spring_damper(
        math::Vector& in_out_position,
        math::Vector& in_out_velocity,
        const math::Vector& position_goal,
        const math::Vector& velocity_goal,
        float halflife,
        float delta_s)
    {
        critical_spring_damper_implicit(in_out_position.x, in_out_velocity.x, position_goal.x, velocity_goal.x, halflife, delta_s);
        critical_spring_damper_implicit(in_out_position.y, in_out_velocity.y, position_goal.y, velocity_goal.y, halflife, delta_s);
    }
}

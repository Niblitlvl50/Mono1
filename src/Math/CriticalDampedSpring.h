
//
// https://theorangeduck.com/page/spring-roll-call#critical
//

#pragma once

#include "Math/Vector.h"
#include "Math/MathFunctions.h"
#include <cmath>

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

    /**
	* Smooths a value using a spring damper towards a target.
	* 
	* The implementation uses approximations for Exp/Sin/Cos. These are accurate for all sensible values of 
	* InUndampedFrequency and DampingRatio so long as InDeltaTime < 1 / InUndampedFrequency (approximately), but
	* are generally well behaved even for larger timesteps etc. 
	* 
	* @param  InOutValue          The value to be smoothed
	* @param  InOutValueRate      The rate of change of the value
	* @param  InTargetValue       The target to smooth towards
	* @param  InTargetValueRate   The target rate of change smooth towards. Note that if this is discontinuous, then the output will have discontinuous velocity too.
	* @param  InDeltaTime         Time interval
	* @param  InUndampedFrequency Oscillation frequency when there is no damping. Proportional to the square root of the spring stiffness.
	* @param  InDampingRatio      1 is critical damping. <1 results in under-damped motion (i.e. with overshoot), and >1 results in over-damped motion. 
	*/
	template< class T >
	inline void SpringDamper(
	    T&          InOutValue,
	    T&          InOutValueRate,
	    const T&    InTargetValue,
	    const T&    InTargetValueRate,
	    const float InDeltaTime,
	    const float InUndampedFrequency,
	    const float InDampingRatio)
	{
		if (InDeltaTime <= 0.0f)
		{
			return;
		}

        constexpr float UE_SMALL_NUMBER = (1.e-8f);

		float W = InUndampedFrequency * math::TAU();
		// Handle special cases
		if (W < UE_SMALL_NUMBER) // no strength which means no damping either
		{
			InOutValue += InOutValueRate * InDeltaTime;
			return;
		}
		else if (InDampingRatio < UE_SMALL_NUMBER) // No damping at all
		{
			T Err = InOutValue - InTargetValue;
			const T B = InOutValueRate / W;
			float S, C;
			math::SinCos(&S, &C, W * InDeltaTime);
			InOutValue = InTargetValue + Err * C + B * S;
			InOutValueRate = InOutValueRate * C - Err * (W * S);
			return;
		}

		// Target velocity turns into an offset to the position
		float SmoothingTime = 2.0f / W;
		T AdjustedTarget = InTargetValue + InTargetValueRate * (InDampingRatio * SmoothingTime);
 		T Err = InOutValue - AdjustedTarget;

		// Handle the cases separately
		if (InDampingRatio > 1.0f) // Overdamped
		{
			const float WD = W * std::sqrt(math::Square(InDampingRatio) - 1.0f);
			const T C2 = -(InOutValueRate + (W * InDampingRatio - WD) * Err) / (2.0f * WD);
			const T C1 = Err - C2;
			const float A1 = (WD - InDampingRatio * W);
			const float A2 = -(WD + InDampingRatio * W);
			// Note that A1 and A2 will always be negative. We will use an approximation for 1/Exp(-A * DeltaTime).
			const float A1_DT = -A1 * InDeltaTime;
			const float A2_DT = -A2 * InDeltaTime;
			// This approximation in practice will be good for all DampingRatios
			const float E1 = math::InvExpApprox(A1_DT);
			// As DampingRatio gets big, this approximation gets worse, but mere inaccuracy for overdamped motion is
			// not likely to be important, since we end up with 1 / BigNumber
			const float E2 = math::InvExpApprox(A2_DT);
			InOutValue = AdjustedTarget + E1 * C1 + E2 * C2;
			InOutValueRate = E1 * C1 * A1 + E2 * C2 * A2;
		}
		else if (InDampingRatio < 1.0f) // Underdamped
		{
			const float WD = W * std::sqrt(1.0f - math::Square(InDampingRatio));
			const T A = Err;
			const T B = (InOutValueRate + Err * (InDampingRatio * W)) / WD;
			float S, C;
			math::SinCos(&S, &C, WD * InDeltaTime);
			const float E0 = InDampingRatio * W * InDeltaTime;
			// Needs E0 < 1 so DeltaTime < SmoothingTime / (2 * DampingRatio * Sqrt(1 - DampingRatio^2))
			const float E = math::InvExpApprox(E0);
			InOutValue = E * (A * C + B * S);
			InOutValueRate = -InOutValue * InDampingRatio * W;
			InOutValueRate += E * (B * (WD * C) - A * (WD * S));
			InOutValue += AdjustedTarget;
		}
		else // Critical damping
		{
			const T& C1 = Err;
			T C2 = InOutValueRate + Err * W;
			const float E0 = W * InDeltaTime;
			// Needs E0 < 1 so InDeltaTime < SmoothingTime / 2 
			float E = math::InvExpApprox(E0);
			InOutValue = AdjustedTarget + (C1 + C2 * InDeltaTime) * E;
			InOutValueRate = (C2 - C1 * W - C2 * (W * InDeltaTime)) * E;
		}
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

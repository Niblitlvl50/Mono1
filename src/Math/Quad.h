
#pragma once

#include "Vector.h"

namespace math
{
    struct Quad
    {
        constexpr Quad()
        { }

        constexpr Quad(float bottom_left_x, float bottom_left_y, float top_right_x, float top_right_y)
            : bottom_left(bottom_left_x, bottom_left_y)
            , top_right(top_right_x, top_right_y)
        { }

        constexpr Quad(const math::Vector& a, const math::Vector& b)
            : bottom_left(a)
            , top_right(b)
        { }

        Quad(const math::Vector& center, float radius)
            : bottom_left(center - math::Vector(radius, radius))
            , top_right(center + math::Vector(radius, radius))
        { }

        Quad(const math::Vector& center, float width, float height)
            : bottom_left(center - math::Vector(width / 2.0f, height / 2.0f))
            , top_right(center + math::Vector(width / 2.0f, height / 2.0f))
        { }

        math::Vector bottom_left;
        math::Vector top_right;
    };
    
    math::Quad operator * (const math::Quad& left, float value);
    math::Quad operator * (const math::Quad& left, const math::Vector& right);
    math::Quad operator + (const math::Quad& quad, const math::Vector& offset);
    
    bool operator == (const math::Quad& left, const math::Quad& right);

    //!
    //! Be aware that this only checks for max/min values.
    //! If you use the Quad as position and size vectors this
    //! will not do what you think. 
    //!
    void ExpandBy(math::Quad& quad, const math::Quad& other);
    void ExpandBy(math::Quad& quad, const math::Vector& point);

    // Flip points if needed to make a quad that goes from less to more
    void NormalizeQuad(math::Quad& quad);

    float Width(const math::Quad& quad);
    float Height(const math::Quad& quad);
    math::Vector Size(const math::Quad& quad);

    float Left(const math::Quad& quad);
    float Right(const math::Quad& quad);
    float Top(const math::Quad& quad);
    float Bottom(const math::Quad& quad);

    math::Vector Center(const math::Quad& quad);

    math::Vector BottomLeft(const math::Quad& quad);
    math::Vector BottomRight(const math::Quad& quad);
    math::Vector BottomCenter(const math::Quad& quad);
    math::Vector TopLeft(const math::Quad& quad);
    math::Vector TopCenter(const math::Quad& quad);
    math::Vector TopRight(const math::Quad& quad);
    math::Vector RightCenter(const math::Quad& quad);
    math::Vector LeftCenter(const math::Quad& quad);

    // Zero quad defined for convenience
    constexpr math::Quad ZeroQuad = math::Quad(math::ZeroVec, math::ZeroVec);

    // Infinite quad, covers everything.
    extern const math::Quad InfQuad;

    // Inverse infinite, covers nothing.
    extern const math::Quad InverseInfQuad;
}

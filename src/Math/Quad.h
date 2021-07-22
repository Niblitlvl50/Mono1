
#pragma once

#include "Vector.h"

namespace math
{
    struct Quad
    {
        constexpr Quad()
        { }

        constexpr Quad(float x, float y, float w, float h)
            : mA(x, y),
              mB(w, h)
        { }

        constexpr Quad(const Vector& a, const Vector& b)
            : mA(a),
              mB(b)
        { }

        Quad(const Vector& center, float radius)
            : mA(center - math::Vector(radius, radius))
            , mB(center + math::Vector(radius, radius))
        { }

        Quad(const Vector& center, float width, float height)
            : mA(center - math::Vector(width / 2.0f, height / 2.0f))
            , mB(center + math::Vector(width / 2.0f, height / 2.0f))
        { }

        Vector mA;
        Vector mB;
    };
    
    Quad operator * (const Quad& left, float value);
    Quad operator * (const Quad& left, const Vector& right);
    
    //!
    //! Be aware that this only checks for max/min values.
    //! If you use the Quad as position and size vectors this
    //! will not do what you think. 
    //!
    void operator |= (Quad& left, const Quad& right);

    void operator |= (Quad& left, const Vector& right);

    bool operator == (const Quad& left, const Quad& right);

    // Flip points if needed to make a quad that goes from less to more
    void NormalizeQuad(math::Quad& quad);

    float Width(const Quad& quad);
    float Height(const Quad& quad);

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

    //! Zero quad defined for convenience
    constexpr Quad ZeroQuad = Quad(ZeroVec, ZeroVec);

    // Infinite quad, covers everything.
    extern const Quad InfQuad;
}

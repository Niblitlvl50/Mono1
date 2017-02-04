
#pragma once

#include "MonoPtrFwd.h"
#include "Math/MathFwd.h"
#include "System/SysTypes.h"
#include <vector>

namespace editor
{
    struct Grabber;

    class IObjectProxy
    {
    public:

        virtual ~IObjectProxy()
        { }

        virtual uint Id() const = 0;
        virtual mono::IEntityPtr Entity() = 0;

        virtual void SetSelected(bool selected) = 0;
        virtual bool Intersects(const math::Vector& position) const = 0;
        virtual std::vector<Grabber> GetGrabbers() const = 0;
    };
}


#pragma once

#include "IEntity.h"
#include "Physics/CMFwd.h"

namespace mono
{
    struct IPhysicsEntity : public IEntity
    {
        virtual ~IPhysicsEntity()
        { }

        virtual mono::Object& GetPhysics() = 0;
    };
}

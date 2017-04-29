
#pragma once

#include "IEntity.h"
#include "Physics/CMFwd.h"

namespace mono
{
    class IPhysicsEntity : public IEntity
    {
    public:

        virtual ~IPhysicsEntity()
        { }

        virtual mono::PhysicsData& GetPhysics() = 0;
    };
}

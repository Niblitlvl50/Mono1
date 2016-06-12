//
//  IPhysicsZone.h
//  MonoiOS
//
//  Created by Niklas Damberg on 19/11/13.
//
//

#pragma once

#include "IZone.h"
#include "MonoPtrFwd.h"
#include "CMFwd.h"

namespace mono
{
    struct IPhysicsZone : public virtual IZone
    {
        virtual void ForEachBody(const cm::BodyFunc& func) = 0;
        virtual IPhysicsEntityPtr FindPhysicsEntityFromBody(const cm::IBodyPtr& body) const = 0;
        virtual mono::IPhysicsEntityPtr FindPhysicsEntityFromId(uint id) const = 0;

        virtual void AddPhysicsEntity(const mono::IPhysicsEntityPtr& entity, int layer) = 0;
        virtual void RemovePhysicsEntity(const mono::IPhysicsEntityPtr& entity) = 0;
    };
}

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
#include <functional>


namespace mono
{
    struct IPhysicsZone : public IZone
    {
        virtual void ForEachBody(const std::function<void (cm::IBodyPtr)>& func) = 0;

        virtual void AddPhysicsEntity(const mono::IPhysicsEntityPtr& entity, int layer) = 0;
        virtual void RemovePhysicsEntity(const mono::IPhysicsEntityPtr& entity) = 0;
    };
}

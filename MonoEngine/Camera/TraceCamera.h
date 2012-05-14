//
//  TraceCamera.h
//  Mono1
//
//  Created by Niblit on 2012-05-11.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ICamera.h"
#include "MonoFwd.h"
#include "Vector2f.h"

namespace mono
{
    class TraceCamera : public mono::ICamera
    {
    public:
        
        TraceCamera(int width, int height);
        
        virtual void Update(unsigned int delta);
        
        virtual void Follow(const mono::IEntityPtr entity);
        virtual void Unfollow();
        
        virtual const Math::Vector2f& Position() const;
        virtual const Math::Vector2f& Size() const;
        
    private:
        
        IEntityPtr mEntity;
        Math::Vector2f mPosition;
        Math::Vector2f mSize;
    };
}

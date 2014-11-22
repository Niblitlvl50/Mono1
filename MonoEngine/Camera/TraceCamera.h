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
#include "Quad.h"
#include "CameraController.h"

namespace mono
{
    class TraceCamera : public mono::ICamera
    {
    public:
        
        TraceCamera(int width, int height, EventHandler& eventHandler);
        
        virtual void Update(unsigned int delta);
        
        virtual void Follow(const mono::IEntityPtr entity);
        virtual void Unfollow();
        
        virtual const math::Quad& GetViewport() const;
        virtual /* */ math::Quad& GetViewport() /* */;
        
        virtual void SetTargetViewport(const math::Quad& target);
        virtual void SetPosition(const math::Vector2f& position);
                        
    private:

        CameraController mController;
        
        IEntityPtr mEntity;
        math::Quad mViewport;
        math::Quad mTargetViewport;
    };
}

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

namespace mono
{
    class TraceCamera : public mono::ICamera
    {
    public:
        
        TraceCamera(int width, int height);
        
        virtual void Update(unsigned int delta);
        
        virtual void Follow(const mono::IEntityPtr& entity, const math::Vector2f& offset);
        virtual void Unfollow();
        
        virtual math::Quad GetViewport() const;
        virtual math::Vector2f GetPosition() const;

        virtual void SetViewport(const math::Quad& viewport);
        virtual void SetTargetViewport(const math::Quad& target);
        virtual void SetPosition(const math::Vector2f& position);

    private:

        IEntityPtr mEntity;
        math::Vector2f m_offset;
        math::Quad mViewport;
        math::Quad mTargetViewport;
    };
}

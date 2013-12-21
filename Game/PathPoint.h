//
//  PathPoint.h
//  MonoOSX
//
//  Created by Niklas Damberg on 21/12/13.
//
//

#pragma once

#include "EntityBase.h"

namespace game
{
    class PathPoint : public mono::EntityBase
    {
    public:
        
        PathPoint();
    
    private:
        
        virtual void Draw(mono::IRenderer& renderer) const;
        virtual void Update(unsigned int delta);
    
        std::shared_ptr<mono::IPath> mPath;
        float mElapsedTime;
    };
}


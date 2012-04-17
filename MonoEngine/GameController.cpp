//
//  GameController.cpp
//  Mono1
//
//  Created by Niblit on 2012-04-06.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "GameController.h"
#include "IZone.h"
#include "Vector2f.h"

#include <stack>
#include <stdexcept>


namespace
{
    class GameController : public mono::IGameController
    {
    public:
        
        virtual void InsertZone(mono::IZonePtr zone)
        {
            mZoneStack.push(zone);        
        }
        
        virtual mono::IZonePtr GetCurrentZone()
        {
            if(mZoneStack.empty())
                throw std::runtime_error("Zone stack is empty, exit");
            
            mono::IZonePtr zone = mZoneStack.top();
            if(zone->IsDone())
            {
                mZoneStack.pop();
                if(mZoneStack.empty())
                    throw std::runtime_error("Zone stack is empty, exit");
                
                zone = mZoneStack.top();
            }
            
            return zone;        
        }
        
        virtual void SetWindowSize(const Math::Vector2f& size)
        {
            mWindowSize = size;
        }
        
        virtual const Math::Vector2f& GetWindowSize() const
        {
            return mWindowSize;
        }
                
        std::stack<mono::IZonePtr> mZoneStack;
        Math::Vector2f mWindowSize;
    };

}

mono::IGameController& mono::GameControllerInstance()
{
    static GameController instance;
    return instance;
}





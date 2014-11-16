//
//  IAction.h
//  Worksample_2
//
//  Created by Niklas Damberg on 09/11/13.
//
//

#pragma once

namespace mono
{
    struct IAction
    {
        virtual ~IAction()
        { }
        
        virtual void Update(unsigned int delta) = 0;
        virtual bool Finished() const = 0;
    };
}

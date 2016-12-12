
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

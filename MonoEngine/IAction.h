
#pragma once

namespace mono
{
    class IAction
    {
    public:
    
        virtual ~IAction()
        { }
        
        virtual void Update(unsigned int delta) = 0;
        virtual bool Finished() const = 0;
    };
}

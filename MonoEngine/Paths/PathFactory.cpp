//
//  PathFactory.cpp
//  King_worksample_OSX
//
//  Created by Niklas Damberg on 13/11/13.
//
//

#include "PathFactory.h"
#include "IPath.h"

#include "LuaState.h"
#include "LuaFunctions.h"
#include "Vector2f.h"

namespace
{
    struct DefaultPath : mono::IPath
    {
        DefaultPath(const std::vector<math::Vector2f>& coords)
            : mPath(coords),
              mLength(0)
        {
            math::Vector2f current = coords.front();
            
            for(auto it = coords.begin(), end = coords.end(); it != end; ++it)
            {
                const math::Vector2f diff = current - *it;
                mLength += math::Length(diff);
            }
        }

        virtual float Length() const
        {
            return mLength;
        }
        
        virtual math::Vector2f GetPositionByLength(float length) const
        {
            float lengthCounter = 0;
            math::Vector2f last = mPath.front();
            if(length == 0)
                return last;
            
            for(auto it = mPath.begin(), end = mPath.end(); it != end; ++it)
            {
                const math::Vector2f& current = *it;
                
                // Do something...
                const math::Vector2f diff = current - last;
                const float diffLength = math::Length(diff);
                lengthCounter += diffLength;
                
                if(lengthCounter >= length)
                {
                    const float prevLength = lengthCounter - diffLength;
                    const float posLength = length - prevLength;
                    const float percent = posLength / diffLength;
                    
                    const math::Vector2f pos = last + (diff * percent);
                    return pos;
                }
                
                last = current;
            }
            
            return math::Vector2f();
        }
        
        const std::vector<math::Vector2f> mPath;
        float mLength;
    };
}

std::shared_ptr<mono::IPath> mono::CreatePath(const std::string& pathFile)
{
    lua::LuaState config(pathFile);
    const lua::MapIntFloatTable table = lua::GetTableMap<int, float>(config, "path");

    std::vector<math::Vector2f> coords;
    for(auto it = table.begin(), end = table.end(); it != end; ++it)
    {
        const std::vector<float>& item = it->second;
        coords.push_back(math::Vector2f(item.at(0), item.at(1)));
    }
    
    return std::make_shared<DefaultPath>(coords);
}

std::shared_ptr<mono::IPath> mono::CreatePath(const std::vector<math::Vector2f>& coords)
{
    return std::make_shared<DefaultPath>(coords);
}





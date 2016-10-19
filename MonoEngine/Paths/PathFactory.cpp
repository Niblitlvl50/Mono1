//
//  PathFactory.cpp
//  Worksample_2
//
//  Created by Niklas Damberg on 13/11/13.
//
//

#include "PathFactory.h"
#include "IPath.h"
#include "Math/Vector2f.h"
#include "System/SysFile.h"

#include "nlohmann_json/json.hpp"

namespace
{
    struct DefaultPath : mono::IPath
    {
        DefaultPath(const std::vector<math::Vector2f>& coords)
            : mPath(coords),
              mLength(0)
        {
            math::Vector2f last = coords.front();
            
            for(const auto& point : coords)
            {
                const math::Vector2f& diff = last - point;
                mLength += math::Length(diff);
                last = point;
            }
        }

        virtual float Length() const
        {
            return mLength;
        }
        
        virtual math::Vector2f GetPositionByLength(float length) const
        {
            math::Vector2f last = mPath.front();
            if(length == 0)
                return last;
            
            float lengthCounter = 0;

            for(const auto& point : mPath)
            {
                const math::Vector2f& diff = point - last;
                const float diffLength = math::Length(diff);
                lengthCounter += diffLength;
                
                if(lengthCounter >= length)
                {
                    const float prevLength = lengthCounter - diffLength;
                    const float posLength = length - prevLength;
                    const float percent = posLength / diffLength;
                    
                    const math::Vector2f& pos = last + (diff * percent);
                    return pos;
                }
                
                last = point;
            }
            
            return math::Vector2f();
        }

        virtual const std::vector<math::Vector2f>& GetPathPoints() const
        {
            return mPath;
        }

        const std::vector<math::Vector2f> mPath;
        float mLength;
    };
}

std::shared_ptr<mono::IPath> mono::CreatePath(const char* path_file)
{
    File::FilePtr file = File::OpenAsciiFile(path_file);
    if(!file)
        return nullptr;

    std::vector<byte> file_data;
    File::FileRead(file, file_data);

    const nlohmann::json& json = nlohmann::json::parse(file_data);

    const std::vector<float>& points = json["path"];

    std::vector<math::Vector2f> coords;
    coords.resize(points.size() / 2);

    std::memcpy(coords.data(), points.data(), sizeof(float) * points.size());
    
    return std::make_shared<DefaultPath>(coords);
}

std::shared_ptr<mono::IPath> mono::CreatePath(const std::vector<math::Vector2f>& coords)
{
    return std::make_shared<DefaultPath>(coords);
}





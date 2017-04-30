
#include "PathFactory.h"
#include "IPath.h"
#include "Math/Vector.h"
#include "System/File.h"

#include "nlohmann_json/json.hpp"

namespace
{
    class DefaultPath : public mono::IPath
    {
    public:

        DefaultPath(const math::Vector& position, const std::vector<math::Vector>& coords)
            : m_position(position),
              m_path(coords),
              m_length(0)
        {
            math::Vector last = coords.front();
            
            for(const auto& point : coords)
            {
                const math::Vector& diff = last - point;
                m_length += math::Length(diff);
                last = point;
            }
        }

        virtual float Length() const
        {
            return m_length;
        }

        virtual const math::Vector& GetGlobalPosition() const
        {
            return m_position;
        }

        virtual void SetGlobalPosition(const math::Vector& position)
        {
            m_position = position;
        }

        virtual math::Vector GetPositionByLength(float length) const
        {
            math::Vector last = m_path.front();
            if(length == 0)
                return last;
            
            float lengthCounter = 0;

            for(const auto& point : m_path)
            {
                const math::Vector& diff = point - last;
                const float diffLength = math::Length(diff);
                lengthCounter += diffLength;
                
                if(lengthCounter >= length)
                {
                    const float prevLength = lengthCounter - diffLength;
                    const float posLength = length - prevLength;
                    const float percent = posLength / diffLength;
                    
                    const math::Vector& pos = last + (diff * percent);
                    return pos;
                }
                
                last = point;
            }
            
            return math::Vector();
        }

        virtual const std::vector<math::Vector>& GetPathPoints() const
        {
            return m_path;
        }

        math::Vector m_position;
        const std::vector<math::Vector> m_path;
        float m_length;
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
    const math::Vector position(json["x"], json["y"]);

    std::vector<math::Vector> coords;
    coords.resize(points.size() / 2);
    std::memcpy(coords.data(), points.data(), sizeof(float) * points.size());
    
    return std::make_shared<DefaultPath>(position, coords);
}

std::shared_ptr<mono::IPath> mono::CreatePath(const math::Vector& position, const std::vector<math::Vector>& coords)
{
    return std::make_shared<DefaultPath>(position, coords);
}

bool mono::SavePath(const char* path_file, const math::Vector& position, const std::vector<math::Vector>& points)
{
    std::vector<float> float_points;
    float_points.resize(points.size() * 2);
    std::memcpy(float_points.data(), points.data(), sizeof(math::Vector) * points.size());

    nlohmann::json json;
    json["x"] = position.x;
    json["y"] = position.y;
    json["path"] = float_points;

    const std::string& serialized_path = json.dump(4);
    File::FilePtr output_file = File::CreateAsciiFile(path_file);
    if(!output_file)
        return false;

    std::fwrite(serialized_path.data(), serialized_path.length(), sizeof(char), output_file.get());
    return true;
}




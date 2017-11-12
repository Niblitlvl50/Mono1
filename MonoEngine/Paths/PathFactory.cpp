
#include "PathFactory.h"
#include "IPath.h"
#include "Math/Vector.h"
#include "System/File.h"

#include "nlohmann_json/json.hpp"

namespace
{
    class PathImpl : public mono::IPath
    {
    public:

        PathImpl(const math::Vector& position, const std::vector<math::Vector>& coords)
            : m_position(position),
              m_points(coords),
              m_total_length(0.0f)
        {
            m_length_table.reserve(coords.size());

            for(int index = 0; index < static_cast<int>(coords.size()); ++index)
            {
                const int previous_index = std::max(0, index -1);

                const math::Vector& previous_point = coords[previous_index];
                const math::Vector& current_point = coords[index];
                
                const math::Vector& diff = current_point - previous_point;

                m_total_length += math::Length(diff);
                m_length_table.push_back(m_total_length);
            }
        }

        float Length() const override
        {
            return m_total_length;
        }

        const math::Vector& GetGlobalPosition() const override
        {
            return m_position;
        }

        void SetGlobalPosition(const math::Vector& position) override
        {
            m_position = position;
        }

        const std::vector<math::Vector>& GetPathPoints() const override
        {
            return m_points;
        }

        math::Vector GetPositionByLength(float length) const override
        {
            if(m_length_table.empty())
                return math::zeroVec;

            const auto find_func = [length](float start_length) {
                return length < start_length;
            };

            auto it = std::find_if(m_length_table.begin(), m_length_table.end(), find_func);
            if(it == m_length_table.end())
                return math::zeroVec;

            --it;

            const size_t point_index = std::distance(m_length_table.begin(), it);

            const math::Vector& previous_point = m_points[point_index];
            const math::Vector& current_point = m_points[point_index +1];

            const math::Vector& diff = current_point - previous_point;
            const float diff_length = math::Length(diff);

            const float local_length = length - (*it);
            const float percent = local_length / diff_length;
            
            const math::Vector& pos = previous_point + (diff * percent);
            return pos;
        }

        math::Vector m_position;
        const std::vector<math::Vector> m_points;

        float m_total_length;
        std::vector<float> m_length_table;
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
    
    return std::make_shared<PathImpl>(position, coords);
}

std::shared_ptr<mono::IPath> mono::CreatePath(const math::Vector& position, const std::vector<math::Vector>& coords)
{
    return std::make_shared<PathImpl>(position, coords);
}

bool mono::SavePath(const char* path_file, const math::Vector& position, const std::vector<math::Vector>& local_points)
{
    std::vector<float> float_points;
    float_points.resize(local_points.size() * 2);
    std::memcpy(float_points.data(), local_points.data(), sizeof(math::Vector) * local_points.size());

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




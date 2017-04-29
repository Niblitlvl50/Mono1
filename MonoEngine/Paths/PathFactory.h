
#pragma once

#include "Math/MathFwd.h"
#include <vector>
#include <memory>

namespace mono
{
    class IPath;
    
    std::shared_ptr<mono::IPath> CreatePath(const char* path_file);
    std::shared_ptr<mono::IPath> CreatePath(const math::Vector& position, const std::vector<math::Vector>& coords);

    bool SavePath(const char* path_file, const math::Vector& position, const std::vector<math::Vector>& points);
}

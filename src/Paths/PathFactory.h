
#pragma once

#include "Math/MathFwd.h"
#include "MonoPtrFwd.h"
#include <vector>
#include <memory>

namespace mono
{
    IPathPtr CreatePath(const char* path_file);
    IPathPtr CreatePath(const math::Vector& position, const std::vector<math::Vector>& coords);

    bool SavePath(const char* path_file, const math::Vector& position, const std::vector<math::Vector>& local_points);
}

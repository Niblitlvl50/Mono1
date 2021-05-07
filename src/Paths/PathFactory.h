
#pragma once

#include "Math/MathFwd.h"
#include "MonoPtrFwd.h"
#include "PathTypes.h"
#include "PathDrawBuffer.h"
#include "Rendering/RenderFwd.h"
#include "Rendering/Color.h"

#include <vector>
#include <memory>

namespace mono
{
    IPathPtr CreatePath(const std::vector<math::Vector>& coords);
    IPathPtr CreatePath(const std::vector<math::Vector>& coords, const math::Matrix& transform);

    enum class UVMode
    {
        DISTANCE,
        NORMALIZED_DISTANCE
    };

    struct PathOptions
    {
        float width;
        mono::Color::RGBA color;
        UVMode uv_mode;
        bool closed;
    };

    PathDrawBuffer BuildPathDrawBuffers(PathType type, const std::vector<math::Vector>& points, const PathOptions& options);
}

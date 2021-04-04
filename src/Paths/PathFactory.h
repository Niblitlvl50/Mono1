
#pragma once

#include "Math/MathFwd.h"
#include "MonoPtrFwd.h"
#include "PathTypes.h"
#include "Rendering/RenderFwd.h"
#include "Rendering/Color.h"

#include <vector>
#include <memory>

namespace mono
{
    IPathPtr CreatePath(const std::vector<math::Vector>& coords);

    struct PathOptions
    {
        float width;
        mono::Color::RGBA color;
        bool closed;
    };

    struct PathDrawBuffer
    {
        std::unique_ptr<IRenderBuffer> vertices;
        std::unique_ptr<IRenderBuffer> colors;
        std::unique_ptr<IRenderBuffer> anotations;
        std::unique_ptr<IElementBuffer> indices;
    };

    PathDrawBuffer BuildPathDrawBuffers(PathType type, const std::vector<math::Vector>& points, const PathOptions& options);
}

//
//  PathFactory.h
//  Worksample_2
//
//  Created by Niklas Damberg on 13/11/13.
//
//

#pragma once

#include "Math/MathFwd.h"
#include <vector>
#include <memory>

namespace mono
{
    struct IPath;
    
    std::shared_ptr<mono::IPath> CreatePath(const char* path_file);
    std::shared_ptr<mono::IPath> CreatePath(const std::vector<math::Vector2f>& coords);
}

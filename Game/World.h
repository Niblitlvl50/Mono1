//
//  World.hpp
//  MonoiOS
//
//  Created by Niklas Damberg on 26/07/16.
//
//

#pragma once

#include "MonoFwd.h"
#include "WorldFile.h"

namespace game
{
    void LoadWorld(mono::IPhysicsZone* zone, const std::vector<world::PolygonData>& polygons);
}

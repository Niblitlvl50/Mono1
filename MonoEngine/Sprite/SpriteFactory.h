//
//  SpriteFactory.hpp
//  MonoiOS
//
//  Created by Niklas Damberg on 21/07/16.
//
//

#pragma once

#include "MonoPtrFwd.h"


namespace mono
{
    ISpritePtr CreateSprite(const char* sprite_file);
}

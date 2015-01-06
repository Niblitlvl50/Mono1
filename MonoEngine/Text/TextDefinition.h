//
//  TextDefinition.h
//  MonoOSX
//
//  Created by Niklas Damberg on 25/12/14.
//
//

#pragma once

#include <vector>
#include "Color.h"

namespace mono
{
    struct TextDefinition
    {
        std::vector<float> vertices;
        std::vector<float> texcoords;
        mono::Color color;
        unsigned int chars;
    };
}

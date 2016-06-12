//
//  SysUID.cpp
//  MonoiOS
//
//  Created by Niklas Damberg on 11/06/16.
//
//

#include "SysUID.h"

namespace
{
    uint uid = 0;
}

uint system::CreateUID()
{
    return uid++;
}

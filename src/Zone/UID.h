
#pragma once

#include <cstdint>

namespace System
{
    // Create a unique id 
    uint32_t CreateUID();
    void SetUIDOffset(uint32_t offset);
}

#pragma once

#include "System/SysTypes.h"

namespace mono
{
    // This is the "FNV-1a alternate algorithm", taken from here: http://isthe.com/chongo/tech/comp/fnv/
    uint Hash(const char* text)
    {
        constexpr uint offset_bias = 2166136261u;
        constexpr uint FNV_prime = 16777619u;

        uint hash = offset_bias;
        const size_t length = std::strlen(text);

        for(uint index = 0; index < length; ++index)
            hash = (hash ^ text[index]) * FNV_prime;

        return hash;
    }
}

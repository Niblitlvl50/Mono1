
#pragma once

#include <cstring>

namespace mono
{
    // This is the "FNV-1a alternate algorithm", taken from here: http://isthe.com/chongo/tech/comp/fnv/
    inline unsigned int Hash(const char* text)
    {
        constexpr unsigned int offset_bias = 2166136261u;
        constexpr unsigned int FNV_prime = 16777619u;

        unsigned int hash = offset_bias;
        const size_t length = std::strlen(text);
        
        for(unsigned int index = 0; index < length; ++index)
            hash = (hash ^ text[index]) * FNV_prime;

        return hash;
    }
}

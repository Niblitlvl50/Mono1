
#pragma once

#include <cstring>
#include <cstdint>

namespace mono
{
    //
    // This is the "FNV-1a alternate algorithm", taken from here: http://isthe.com/chongo/tech/comp/fnv/
    //
    inline uint32_t Hash(const char* text, uint32_t length)
    {
        constexpr uint32_t offset_bias = 2166136261u;
        constexpr uint32_t FNV_prime = 16777619u;

        uint32_t hash = offset_bias;

        for(uint32_t index = 0; index < length; ++index)
            hash = (hash ^ text[index]) * FNV_prime;

        return hash;
    }

    inline uint32_t Hash(const char* text)
    {
        const std::size_t length = std::strlen(text);
        return Hash(text, length);
    }

    void HashRegisterString(const char* string);
    const char* HashLookup(uint32_t hash_value);
}

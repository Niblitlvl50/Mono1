
#pragma once

#include <vector>

namespace mono
{
    constexpr uint32_t INVALID_ID = -1;

    struct Entity
    {
        uint32_t id = INVALID_ID;
        uint32_t properties = 0;        // Bitset of properties
        std::vector<uint32_t> components;
    };
}

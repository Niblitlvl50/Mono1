
#pragma once

#include <cstdint>

namespace mono
{
    using BitFieldTextCallback = const char*(*)(uint32_t);
    bool DrawBitFieldType(
        uint32_t& value, const uint32_t* possible_properties, uint32_t n_properties, BitFieldTextCallback text_callback);
}

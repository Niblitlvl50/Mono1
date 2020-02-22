
#pragma once

#include <cstdint>

namespace mono
{
    using BitFieldTextCallback = const char*(*)(uint32_t);
    void DrawBitFieldType(
        uint32_t& properties, const uint32_t* possible_properties, uint32_t n_properties, BitFieldTextCallback text_callback);
}

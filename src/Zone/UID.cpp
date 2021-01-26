
#include "UID.h"

namespace
{
    uint32_t uid = 0;
}

uint32_t System::CreateUID()
{
    return uid++;
}

void System::SetUIDOffset(uint32_t offset)
{
    uid = offset;
}

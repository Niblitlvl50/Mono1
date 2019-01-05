
#include "UID.h"

namespace
{
    uint32_t uid = 0;
}

uint32_t System::CreateUID()
{
    return uid++;
}

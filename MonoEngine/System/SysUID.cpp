
#include "SysUID.h"

namespace
{
    unsigned int uid = 0;
}

unsigned int system::CreateUID()
{
    return uid++;
}

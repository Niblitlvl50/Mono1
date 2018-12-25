
#include "UID.h"

namespace
{
    unsigned int uid = 0;
}

unsigned int System::CreateUID()
{
    return uid++;
}

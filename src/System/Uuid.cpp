
#include "Uuid.h"
#include "System/Hash.h"

#include "uuid4/src/uuid4.h"

#include <cassert>

uint32_t uuid::Uuid4Hashed()
{
    {
        static const int init_result = []{
            return uuid4_init();
        }();
        (void)init_result;
        assert(init_result == UUID4_ESUCCESS);
    }

    char uuid_string[UUID4_LEN];
    uuid4_generate(uuid_string);

    return hash::Hash(uuid_string);
}

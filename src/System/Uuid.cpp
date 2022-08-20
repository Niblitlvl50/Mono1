
#include "Uuid.h"
#include "System/Hash.h"
#include "System/Debug.h"

#include "uuid4/src/uuid4.h"

uint32_t uuid::Uuid4Hashed()
{
    {
        static const int init_result = []{
            return uuid4_init();
        }();
        (void)init_result;
        MONO_ASSERT(init_result == UUID4_ESUCCESS);
    }

    char uuid_string[UUID4_LEN];
    uuid4_generate(uuid_string);

    return hash::Hash(uuid_string);
}

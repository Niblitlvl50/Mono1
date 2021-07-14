
#include "System/Uuid.h"
#include "gtest/gtest.h"

#include <unordered_set>

TEST(UuidTest, GenerateUuids)
{
    std::unordered_set<uint32_t> hashed_uuids;

    for(int index = 0; index < 1000; ++index)
    {
        const auto result = hashed_uuids.insert(uuid::Uuid4Hashed());
        ASSERT_TRUE(result.second);
    }
}

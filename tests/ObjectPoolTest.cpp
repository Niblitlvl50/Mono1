
#include "Util/ObjectPool.h"
#include "gtest/gtest.h"

namespace
{
    struct TestData
    {
        int value = 0;
    };
}

TEST(ObjectPoolTest, HashStringAndCompare)
{
    mono::ObjectPool<TestData> pool(500);

    TestData* data = pool.GetPoolData();
    data->value = 7;

    ASSERT_EQ(1ul, pool.Used());

    pool.ReleasePoolData(data);
    ASSERT_EQ(0ul, pool.Used());

    TestData* same_data = pool.GetPoolData();
    ASSERT_EQ(7, same_data->value);
}


#include "Util/ActiveVector.h"
#include "gtest/gtest.h"


TEST(ActiveVectorTest, ForEach)
{
    mono::ActiveVector<float> my_stuff(500);

    my_stuff.Set(1, 7.0f);

    int count_1 = 0;
    my_stuff.ForEach([&count_1](uint32_t index, const float& float_data) {
        count_1++;
    });
    ASSERT_EQ(count_1, 1);

    my_stuff.Release(1);

    int count_2 = 0;
    my_stuff.ForEach([&count_2](uint32_t index, const float& data){
        count_2++;
    });
    ASSERT_EQ(count_2, 0);
}

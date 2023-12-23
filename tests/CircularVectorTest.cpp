
#include "Util/CircularVector.h"
#include <gtest/gtest.h>

TEST(CircularVector, test1)
{
    mono::CircularVector<int, 10> circular_vector;

    for(int index = 0; index < 20; ++index)
        circular_vector.Push(index);

    const auto print_value_callback = [](int value) {
        printf("%d ", value);
    };

    circular_vector.ForEach(print_value_callback);
    printf("\n");


    for(uint32_t index = 0; index < circular_vector.Length(); ++index)
        printf("%d ", circular_vector[index]);

    printf("\n");
}


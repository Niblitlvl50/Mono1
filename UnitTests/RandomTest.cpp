//
//  RandomTest.cpp
//  MonoOSX
//
//  Created by Niklas Damberg on 22/11/14.
//
//

#include "Utils.h"
#include <gtest/gtest.h>

TEST(RandomTest, ValidateRandomValueInsideRange0to1)
{
    for(int index = 0; index < 1000; ++index)
    {
        const float random = mono::Random();
        EXPECT_GT(random, 0.0f);
        EXPECT_LT(random, 1.0f);
    }
}

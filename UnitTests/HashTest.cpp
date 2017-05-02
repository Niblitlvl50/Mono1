
#include "Hash.h"
#include "gtest/gtest.h"

TEST(HashTest, HashStringAndCompare)
{
    constexpr const char* string_to_hash = "hello";
    constexpr const char* another_string = "Whatever!";

    const unsigned int first_hash = mono::Hash(string_to_hash);
    const unsigned int second_hash = mono::Hash(string_to_hash);
    const unsigned int third_hash = mono::Hash(another_string);

    EXPECT_EQ(first_hash, second_hash);
    EXPECT_NE(first_hash, third_hash);
}


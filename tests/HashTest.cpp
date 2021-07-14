
#include "System/Hash.h"
#include "gtest/gtest.h"

TEST(HashTest, HashStringAndCompare)
{
    constexpr const char* string_to_hash = "hello";
    constexpr const char* another_string = "Whatever!";

    const uint32_t first_hash = hash::Hash(string_to_hash);
    const uint32_t second_hash = hash::Hash(string_to_hash);
    const uint32_t third_hash = hash::Hash(another_string);

    EXPECT_EQ(first_hash, second_hash);
    EXPECT_NE(first_hash, third_hash);
}


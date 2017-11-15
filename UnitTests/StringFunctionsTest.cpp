
#include "StringFunctions.h"
#include <gtest/gtest.h>

TEST(StringFunctionsTest, Split)
{
    constexpr const char* string_1 = "hello banan boat!";

    const std::vector<std::string>& tokens = mono::SplitString(string_1, ' ');

    EXPECT_EQ(size_t(3), tokens.size());
    EXPECT_STREQ("hello", tokens[0].c_str());
    EXPECT_STREQ("banana", tokens[1].c_str());
    EXPECT_STREQ("boat!", tokens[2].c_str());
}

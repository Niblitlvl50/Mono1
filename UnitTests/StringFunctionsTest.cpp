
#include "Util/StringFunctions.h"
#include <gtest/gtest.h>

TEST(StringFunctionsTest, SplitMultipleTokens)
{
    constexpr const char* string_1 = "hello banana boat!";
    const std::vector<std::string>& tokens = mono::SplitString(string_1, ' ');

    EXPECT_EQ(3u, tokens.size());
    EXPECT_STREQ("hello", tokens[0].c_str());
    EXPECT_STREQ("banana", tokens[1].c_str());
    EXPECT_STREQ("boat!", tokens[2].c_str());
}

TEST(StringFunctionsTest, SplitSingleToken)
{
    constexpr const char* string_1 = "whatever";
    const std::vector<std::string>& tokens = mono::SplitString(string_1, ' ');

    EXPECT_EQ(1u, tokens.size());
    EXPECT_STREQ(string_1, tokens[0].c_str());
}

TEST(StringFunctionsTest, SplitEmptyString)
{
    const std::vector<std::string>& tokens_1 = mono::SplitString("", ' ');
    EXPECT_TRUE(tokens_1.empty());
}

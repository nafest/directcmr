#include "utils.h"
#include "gtest/gtest.h"

TEST(split_string, works_correctly) {
    std::string in = "the lazy fox";
    auto result = split_string(in, ' ');

    EXPECT_STREQ("the", result[0].c_str());
    EXPECT_STREQ("lazy", result[1].c_str());
    EXPECT_STREQ("fox", result[2].c_str());
}

TEST(split_string, removes_trailing_spaces) {
    std::string in = "Hello ";
    auto result = split_string(in, ' ');
    EXPECT_EQ(1, result.size());
    EXPECT_STREQ("Hello", result[0].c_str());
}

TEST(split_string, returns_single_chars_at_the_end) {
    std::string in = "Hello Y";
    auto result = split_string(in, ' ');
    EXPECT_EQ(2, result.size());
    EXPECT_STREQ("Hello", result[0].c_str());
    EXPECT_STREQ("Y", result[1].c_str());
}


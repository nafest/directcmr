#include "utils.h"
#include "gtest/gtest.h"

TEST(split_string, works_correctly) {
    std::string in = "the lazy fox";
    auto result = split_string(in, ' ');
    
    EXPECT_STREQ("the", result[0].c_str());
    EXPECT_STREQ("lazy", result[1].c_str());
    EXPECT_STREQ("fox", result[2].c_str());
}

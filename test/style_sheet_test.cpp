#include "style_sheet.h"
#include "gtest/gtest.h"

TEST(split_key_value, splits_correctly1) {
    auto kv = "code.margin: 2.f";
    auto split = dcmr::split_key_value(kv);

    EXPECT_STREQ("code.margin", split.first.c_str());
    EXPECT_STREQ("2.f", split.second.c_str());
}

TEST(split_key_value, splits_correctly2) {
    auto kv = " code.margin : 2.f\n";
    auto split = dcmr::split_key_value(kv);

    EXPECT_STREQ("code.margin", split.first.c_str());
    EXPECT_STREQ("2.f", split.second.c_str());
}

TEST(split_key_value, splits_correctly3) {
    auto kv = " code.margin : 2.f \n";
    auto split = dcmr::split_key_value(kv);

    EXPECT_STREQ("code.margin", split.first.c_str());
    EXPECT_STREQ("2.f", split.second.c_str());
}

TEST(split_key_value, splits_correctly4) {
    auto kv = "code.margin:#abc";
    auto split = dcmr::split_key_value(kv);

    EXPECT_STREQ("code.margin", split.first.c_str());
    EXPECT_STREQ("#abc", split.second.c_str());
}

TEST(set_params, sets_multiple_params) {
    dcmr::style_sheet sheet;

    auto params = "code.margin_top: 42.0f\ncode.font: foo";
    sheet.set_params(params);

    EXPECT_EQ(42.f, sheet.get_float_param("code.margin_top"));
    EXPECT_STREQ("foo", sheet.get_string_param("code.font").c_str());
}
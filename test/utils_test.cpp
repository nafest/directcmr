#include "utils.h"
#include "backend.h"
#include "gtest/gtest.h"

TEST(line_splitter, single) {
    std::string str = "foo";
    cmr::line_splitter splitter(str);

    auto begin = splitter.begin();
    std::string first = *begin;
    EXPECT_STREQ("foo", first.c_str());
}

TEST(line_splitter, splits_two_lines) {
    std::string str = "foo\nbar";
    std::vector<std::string> lines;
    for (auto &line : cmr::line_splitter(str))
        lines.push_back(line);

    ASSERT_EQ(2, lines.size());
    EXPECT_STREQ("foo", lines[0].c_str());
    EXPECT_STREQ("bar", lines[1].c_str());
}

std::vector<std::string> split_words(const std::string &txt) {
    cmr::word_splitter splitter(txt);
    std::vector<std::string> word_vec;
    for (auto w : splitter)
        word_vec.push_back(w);

    return word_vec;
}

TEST(word_splitter, empty_string) {
    std::string str = "";
    auto words = split_words(str);

    EXPECT_EQ(0, words.size());
}

TEST(word_splitter, whitespace_string) {
    std::string str = " ";
    auto words = split_words(str);

    EXPECT_EQ(0, words.size());

    str = "  ";
    words = split_words(str);
    EXPECT_EQ(0, words.size());
}

TEST(word_splitter, single_word) {
    std::string str = "foo";
    auto words = split_words(str);

    EXPECT_EQ(1, words.size());
    EXPECT_STREQ("foo", words[0].c_str());
}

TEST(word_splitter, leading_whitespace) {
    std::string str = " foo";
    auto words = split_words(str);

    EXPECT_EQ(1, words.size());
    EXPECT_STREQ("foo", words[0].c_str());
}

TEST(word_splitter, trailing_whitespace) {
    std::string str = "foo ";
    auto words = split_words(str);

    EXPECT_EQ(1, words.size());
    EXPECT_STREQ("foo", words[0].c_str());
}

TEST(word_splitter, leading_and_trailing_whitespace) {
    std::string str = "  foo  ";
    auto words = split_words(str);

    EXPECT_EQ(1, words.size());
    EXPECT_STREQ("foo", words[0].c_str());
}

TEST(word_splitter, two_words) {
    std::string str = "foo bar";
    auto words = split_words(str);

    EXPECT_EQ(2, words.size());
    EXPECT_STREQ("foo", words[0].c_str());
    EXPECT_STREQ("bar", words[1].c_str());
}

TEST(word_splitter, two_words_multiple_whitespace) {
    std::string str = "foo   bar";
    auto words = split_words(str);

    EXPECT_EQ(2, words.size());
    EXPECT_STREQ("foo", words[0].c_str());
    EXPECT_STREQ("bar", words[1].c_str());
}

TEST(word_splitter, first_token_with_single_word) {
    size_t start = 0;
    size_t end = 0;

    cmr::word::first_token("foo", start, end);
    EXPECT_EQ(0, start);
    EXPECT_EQ(std::string::npos, end);
}

TEST(string_to_color, valid_string) {
    auto c = cmr::string_to_color("#010203ff");
    EXPECT_EQ(1, c.r);
    EXPECT_EQ(2, c.g);
    EXPECT_EQ(3, c.b);
    EXPECT_EQ(255, c.a);
}

TEST(string_to_color, invalid_string) {
    auto c = cmr::string_to_color("");
    EXPECT_EQ(0, c.r);
    EXPECT_EQ(0, c.g);
    EXPECT_EQ(0, c.b);
    EXPECT_EQ(255, c.a);
}

TEST(string_to_color, invalid_string2) {
    auto c = cmr::string_to_color("invalid_color");
    EXPECT_EQ(0, c.r);
    EXPECT_EQ(0, c.g);
    EXPECT_EQ(0, c.b);
    EXPECT_EQ(255, c.a);
}

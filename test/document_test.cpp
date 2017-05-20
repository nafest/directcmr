#include "document.h"
#include "fake_backend.h"
#include "gtest/gtest.h"

class document_test : public ::testing::Test {
  public:
    document_test() : ::testing::Test() {}

  protected:
    virtual void SetUp() override {
        // reset some preset stylings to ease testing
        auto& ss = fbcknd.get_style_sheet();
        ss.set_float_param("margin_top", 0.f);
        ss.set_float_param("margin_bottom", 0.f);
        ss.set_float_param("document.margin", 0.f);
    }

    virtual void TearDown() override {}

    fake_backend fbcknd;
};

TEST(document, return_document_for_invalid_file_name) {
    dcmr::document d = dcmr::document::from_file("definitily_not_there");
    EXPECT_NE(nullptr, d.get_root_element());
}

TEST(document, emph_style_is_propagated) {
    auto d = dcmr::document::from_string("Hello *world*");
    auto e = d.get_root_element();
    auto paragraph = e->children()[0];
    auto hello = paragraph->children()[0];
    auto emph = paragraph->children()[1];
    auto world = emph->children()[0];

    EXPECT_FALSE(hello->get_style().get_emph());
    EXPECT_TRUE(world->get_style().get_emph());
}

TEST(document, strong_style_is_propagated) {
    auto d = dcmr::document::from_string("Hello **world**");
    auto e = d.get_root_element();
    auto paragraph = e->children()[0];
    auto hello = paragraph->children()[0];
    auto emph = paragraph->children()[1];
    auto world = emph->children()[0];

    EXPECT_FALSE(hello->get_style().get_strong());
    EXPECT_FALSE(hello->get_style().get_emph());
    EXPECT_TRUE(world->get_style().get_strong());
    EXPECT_FALSE(world->get_style().get_emph());
}

TEST_F(document_test, layouting_of_a_single_line_is_correct) {
    auto d = dcmr::document::from_string("Hello World");
    d.set_backend(&fbcknd);
    EXPECT_EQ(d.get_root_element()->get_font(&fbcknd)->get_line_height(),
              d.layout(400));
}

TEST_F(document_test, layouting_of_a_single_heading_is_correct) {
    auto d = dcmr::document::from_string("# Hello World");
    d.set_backend(&fbcknd);
    dcmr::style st;
    st.set_heading_level(1);
    auto fnt = fbcknd.font_for_style(st);
    EXPECT_EQ(fnt->get_line_height(), d.layout(400));
}

TEST_F(document_test, layouting_of_to_large_single_word_is_correct) {
    auto d = dcmr::document::from_string("# HelloWorld");
    d.set_backend(&fbcknd);
    dcmr::style st;
    st.set_heading_level(1);
    auto fnt = fbcknd.font_for_style(st);
    // the height of the layout must not span two lines, even
    // though the 50 pixels are not enough to fit the heading
    EXPECT_EQ(fnt->get_line_height(), d.layout(50));
}

TEST_F(document_test, list_layouting_is_correct) {
    auto& ss = fbcknd.get_style_sheet();
    ss.set_float_param("list.margin_left", 5.f);
    dcmr::document d = dcmr::document::from_string("- Item1\n- Item2");
    d.set_backend(&fbcknd);
    auto height = d.layout(400);
    EXPECT_EQ(2 * d.get_root_element()->get_font(&fbcknd)->get_line_height(),
              height);
    auto e = d.get_root_element();
    auto list = e->children()[0];
    EXPECT_STREQ("list", list->get_type().c_str());
    auto item1 = list->children()[0];
    EXPECT_STREQ("item", item1->get_type().c_str());
    auto item2 = list->children()[1];

    auto indent = ss.get_float_param("list.margin_left");

    EXPECT_EQ(indent, item1->get_position().x());
    EXPECT_EQ(indent, item2->get_position().x());
}

TEST_F(document_test, code_layout_is_correct) {
    auto& ss = fbcknd.get_style_sheet();
    ss.set_float_param("code_block.margin_left", 10.f);
    dcmr::document d = dcmr::document::from_string("    code;");
    d.set_backend(&fbcknd);
    auto height = d.layout(400);
    EXPECT_EQ(d.get_root_element()->get_font(&fbcknd)->get_line_height() +
                  ss.get_float_param("code_block.margin_top") +
                  ss.get_float_param("code_block.margin_bottom"),
              height);
    auto root = d.get_root_element();
    auto code = root->children()[0];
    EXPECT_STREQ("code_block", code->get_type().c_str());

    // the margin is applied when rendering
    EXPECT_EQ(0.f, code->get_position().x());
}

TEST_F(document_test, code_font_is_propagated) {
    auto& ss = fbcknd.get_style_sheet();
    ss.set_float_param("code_block.margin_left", 10.f);
    dcmr::document d = dcmr::document::from_string("    code;");
    d.set_backend(&fbcknd);
    auto height = d.layout(400);
    EXPECT_EQ(d.get_root_element()->get_font(&fbcknd)->get_line_height() +
                  ss.get_float_param("code_block.margin_top") +
                  ss.get_float_param("code_block.margin_bottom"),
              height);
    auto root = d.get_root_element();
    auto code = root->children()[0];
    fake_font *fnt = static_cast<fake_font *>(code->get_font(&fbcknd));
    ASSERT_NE(nullptr, fnt);
    EXPECT_STREQ(ss.get_string_param("code_block.font").c_str(),
                 fnt->m_font_params.m_family.c_str());
}

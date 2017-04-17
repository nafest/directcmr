#include "document.h"
#include "fake_renderer.h"
#include "gtest/gtest.h"

class document_test : public ::testing::Test {
public:
  document_test() : ::testing::Test() {}

protected:
  virtual void SetUp() override {
      // reset some preset stylings to ease testing
      frndr.set_float_param("margin_top", 0.f);
      frndr.set_float_param("margin_bottom", 0.f);
      frndr.set_float_param("document.margin", 0.f);
  }

  virtual void TearDown() override {}

  fake_renderer frndr;
};

TEST(document, return_document_for_invalid_file_name) {
    document d = document::from_file("definitily_not_there");
    EXPECT_NE(nullptr, d.get_root_element());
}

TEST(document, emph_style_is_propagated) {
    document d = document::from_string("Hello *world*");
    element *e = d.get_root_element();
    element *paragraph = e->children()[0];
    element *hello = paragraph->children()[0];
    element *emph = paragraph->children()[1];
    element *world = emph->children()[0];

    EXPECT_FALSE(hello->get_style().get_emph());
    EXPECT_TRUE(world->get_style().get_emph());
}

TEST(document, strong_style_is_propagated) {
    document d = document::from_string("Hello **world**");
    element *e = d.get_root_element();
    element *paragraph = e->children()[0];
    element *hello = paragraph->children()[0];
    element *emph = paragraph->children()[1];
    element *world = emph->children()[0];

    EXPECT_FALSE(hello->get_style().get_strong());
    EXPECT_FALSE(hello->get_style().get_emph());
    EXPECT_TRUE(world->get_style().get_strong());
    EXPECT_FALSE(world->get_style().get_emph());
}

TEST_F(document_test, layouting_of_a_single_line_is_correct) {
    document d = document::from_string("Hello World");
    d.set_renderer(&frndr);
    EXPECT_EQ(d.get_root_element()->get_font(&frndr)->get_line_height(),
              d.layout(400));
}

TEST_F(document_test, layouting_of_a_single_heading_is_correct) {
    document d = document::from_string("# Hello World");
    d.set_renderer(&frndr);
    style st;
    st.set_heading_level(1);
    auto fnt = frndr.font_for_style(st);
    EXPECT_EQ(fnt->get_line_height(), d.layout(400));
}

TEST_F(document_test, layouting_of_to_large_single_word_is_correct) {
    document d = document::from_string("# HelloWorld");
    d.set_renderer(&frndr);
    style st;
    st.set_heading_level(1);
    auto fnt = frndr.font_for_style(st);
    // the height of the layout must not span two lines, even
    // though the 50 pixels are not enough to fit the heading
    EXPECT_EQ(fnt->get_line_height(), d.layout(50));
}

TEST_F(document_test, list_layouting_is_correct) {
    frndr.set_float_param("list.margin_left", 5.f);
    document d = document::from_string("- Item1\n- Item2");
    d.set_renderer(&frndr);
    auto height = d.layout(400);
    EXPECT_EQ(2 * d.get_root_element()->get_font(&frndr)->get_line_height(),
              height);
    auto e = d.get_root_element();
    auto list = e->children()[0];
    EXPECT_STREQ("list", list->get_type().c_str());
    auto item1 = list->children()[0];
    EXPECT_STREQ("item", item1->get_type().c_str());
    auto item2 = list->children()[1];

    auto indent = frndr.get_float_param("list.margin_left");

    EXPECT_EQ(indent, item1->get_position().x());
    EXPECT_EQ(indent, item2->get_position().x());
}

TEST_F(document_test, code_layout_is_correct) {
    frndr.set_float_param("code_block.margin_left", 10.f);
    document d = document::from_string("    code;");
    d.set_renderer(&frndr);
    auto height = d.layout(400);
    EXPECT_EQ(d.get_root_element()->get_font(&frndr)->get_line_height() +
                  frndr.get_float_param("code_block.margin_top") +
                  frndr.get_float_param("code_block.margin_bottom"),
              height);
    auto root = d.get_root_element();
    auto code = root->children()[0];
    EXPECT_STREQ("code_block", code->get_type().c_str());

    // the margin is applied when rendering
    EXPECT_EQ(0.f, code->get_position().x());
}

TEST_F(document_test, code_font_is_propagated) {
    frndr.set_float_param("code_block.margin_left", 10.f);
    document d = document::from_string("    code;");
    d.set_renderer(&frndr);
    auto height = d.layout(400);
    EXPECT_EQ(d.get_root_element()->get_font(&frndr)->get_line_height() +
                  frndr.get_float_param("code_block.margin_top") +
                  frndr.get_float_param("code_block.margin_bottom"),
              height);
    auto root = d.get_root_element();
    auto code = root->children()[0];
    fake_font *fnt = static_cast<fake_font *>(code->get_font(&frndr));
    ASSERT_NE(nullptr, fnt);
    EXPECT_STREQ(frndr.get_string_param("code_block.font").c_str(),
                 fnt->m_font_params.m_family.c_str());
}

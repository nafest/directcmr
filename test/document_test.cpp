#include "document.h"
#include "fake_renderer.h"
#include "gtest/gtest.h"

TEST(document, create_document) {
    document d = document::fromFile("foo.md");
    EXPECT_TRUE(true);
}

TEST(document, emph_style_is_propagated) {
    document d = document::fromString("Hello *world*");
    element *e = d.get_root_element();
    element *paragraph = e->children()[0];
    element *hello = paragraph->children()[0];
    element *emph = paragraph->children()[1];
    element *world = emph->children()[0];

    EXPECT_FALSE(hello->get_style().get_emph());
    EXPECT_TRUE(world->get_style().get_emph());
}

TEST(document, strong_style_is_propagated) {
    document d = document::fromString("Hello **world**");
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

TEST(document, layouting_of_a_single_line_is_correct) {
    fake_renderer frndr;
    document d = document::fromString("Hello World");
    d.set_renderer(&frndr);
    EXPECT_EQ(d.get_root_element()->get_font(&frndr)->get_line_height(),
              d.layout(400));
}

TEST(document, layouting_of_a_single_heading_is_correct) {
    fake_renderer frndr;
    document d = document::fromString("# Hello World");
    d.set_renderer(&frndr);
    style st;
    st.set_heading_level(1);
    auto fnt = frndr.font_for_style(st);
    EXPECT_EQ(fnt->get_line_height(), d.layout(400));
}

TEST(document, list_layouting_is_correct) {
    fake_renderer frndr;
    frndr.set_float_param("list_margin", 5.f);
    document d = document::fromString("- Item1\n- Item2");
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

    auto indent = frndr.get_float_param("list_margin");

    EXPECT_EQ(indent, item1->get_position().x());
    EXPECT_EQ(indent, item2->get_position().x());
}

TEST(document, code_layout_is_correct) {
    fake_renderer frndr;
    frndr.set_float_param("code_margin", 10.f);
    document d = document::fromString("    code;");
    d.set_renderer(&frndr);
    auto height = d.layout(400);
    EXPECT_EQ(d.get_root_element()->get_font(&frndr)->get_line_height(),
              height);
    auto root = d.get_root_element();
    auto code = root->children()[0];
    EXPECT_STREQ("code_block", code->get_type().c_str());

    EXPECT_EQ(10.0, code->get_position().x());
}

TEST(document, code_font_is_propagated) {
    fake_renderer frndr;
    frndr.set_float_param("code_margin", 10.f);
    document d = document::fromString("    code;");
    d.set_renderer(&frndr);
    auto height = d.layout(400);
    EXPECT_EQ(d.get_root_element()->get_font(&frndr)->get_line_height(),
              height);
    auto root = d.get_root_element();
    auto code = root->children()[0];
    fake_font *fnt = static_cast<fake_font *>(code->get_font(&frndr));
    ASSERT_NE(nullptr, fnt);
    EXPECT_STREQ(frndr.get_string_param("code_block.font").c_str(),
                 fnt->m_font_params.m_family.c_str());
}

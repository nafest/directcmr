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

TEST(document, list_layouting_is_correct) {
    fake_renderer frndr;
    document d = document::fromString("- Item1\n- Item2");
    d.set_renderer(&frndr);
    d.layout(400);

    auto e = d.get_root_element();
    auto list = e->children()[0];
    EXPECT_STREQ("list", list->get_type().c_str());
    auto item1 = list->children()[0];
    EXPECT_STREQ("item", item1->get_type().c_str());
}

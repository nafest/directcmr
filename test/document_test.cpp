#include "gtest/gtest.h"
#include "document.h"

TEST(document, create_document)
{
    document d = document::fromFile("foo.md");
    EXPECT_TRUE(true);
}

TEST(document, emph_style_is_propagated)
{
    document d = document::fromString("Hello *world*");
    element *e = d.get_root_element();
    element *paragraph = e->children()[0];
    element *hello = paragraph->children()[0];
    element *emph = paragraph->children()[1];
    element *world = emph->children()[0];

    EXPECT_FALSE(hello->get_style().get_emph());
    EXPECT_TRUE(world->get_style().get_emph());
}

TEST(document, strong_style_is_propagated)
{
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

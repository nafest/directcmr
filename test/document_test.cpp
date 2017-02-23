#include "gtest/gtest.h"
#include "document.h"

TEST(document, create_document)
{
    document d = document::fromFile("foo.md");
    EXPECT_TRUE(true);
}

#include "document.h"
#include "fake_backend.h"
#include "gtest/gtest.h"

TEST(thematic_break_element, renders_correctly) {
    fake_backend fbcknd;

    // reset some preset stylings to ease testing
    fbcknd.set_float_param("margin_top", 0.f);
    fbcknd.set_float_param("margin_bottom", 0.f);
    fbcknd.set_float_param("document.margin", 0.f);
    fbcknd.set_float_param("thematic_break.line_width", 1.f);

    auto doc = cmr::document::from_string("Hello\n\n   ---");
    doc.set_backend(&fbcknd);
    doc.layout(500);
    doc.render(cmr::vec2(0, 0), 500);

    ASSERT_EQ(1, fbcknd.m_draw_line_calls.size());
    EXPECT_EQ(cmr::vec2(0.0, 16.5), fbcknd.m_draw_line_calls[0].m_from);
    EXPECT_EQ(cmr::vec2(500.0, 16.5), fbcknd.m_draw_line_calls[0].m_to);
}
#include "document.h"
#include "fake_renderer.h"
#include "gtest/gtest.h"

TEST(thematic_break_element, renders_correctly) {
    fake_renderer frndr;

    // reset some preset stylings to ease testing
    frndr.set_float_param("margin_top", 0.f);
    frndr.set_float_param("margin_bottom", 0.f);
    frndr.set_float_param("document.margin", 0.f);
    frndr.set_float_param("thematic_break.line_width", 1.f);

    document doc = document::from_string("Hello\n\n   ---");
    doc.set_renderer(&frndr);
    doc.layout(500);
    doc.render(vec2(0, 0), 500);

    ASSERT_EQ(1, frndr.m_draw_line_calls.size());
    EXPECT_EQ(vec2(0.0, 16.5), frndr.m_draw_line_calls[0].m_from);
    EXPECT_EQ(vec2(500.0, 16.5), frndr.m_draw_line_calls[0].m_to);
}
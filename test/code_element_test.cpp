#include "document.h"
#include "fake_renderer.h"
#include "gtest/gtest.h"

TEST(code_elment, inline_code_element) {
    fake_renderer frndr;

    // reset some preset stylings to ease testing
    frndr.set_float_param("margin_top", 0.f);
    frndr.set_float_param("margin_bottom", 0.f);
    frndr.set_float_param("document.margin", 0.f);
    frndr.set_float_param("image.margin_left", 2.f);
    frndr.set_float_param("image.margin_right", 2.f);
    frndr.set_float_param("code.margin_left", 5.f);
    frndr.set_float_param("code.margin_right", 5.f);
    frndr.set_string_param("code.font", "test_code_font");
    frndr.set_string_param("code.background_color", "#aabbccdd");

    document doc = document::from_string("Hello `code`");
    doc.set_renderer(&frndr);
    doc.layout(500);
    doc.render(vec2(0, 0), 500);

    ASSERT_EQ(2, frndr.m_draw_string_calls.size());

    // the second string must use the code font
    EXPECT_STREQ("test_code_font",
                 frndr.m_draw_string_calls[1].m_font.m_family.c_str());

    // both words must be rendered on the same base line
    EXPECT_EQ(frndr.m_draw_string_calls[0].m_pos.y(),
              frndr.m_draw_string_calls[1].m_pos.y());

    EXPECT_EQ(65.f, frndr.m_draw_string_calls[1].m_pos.x());

    // test if the background rectangle is positioned correctly
    EXPECT_EQ(1, frndr.m_draw_rect_calls.size());

    EXPECT_EQ(vec2(60.0, 0.0), frndr.m_draw_rect_calls[0].m_rect.top_left());
    // the rectangle must include the 5+5px margin
    EXPECT_EQ(vec2(110.0, 16.0),
              frndr.m_draw_rect_calls[0].m_rect.bottom_right());
}

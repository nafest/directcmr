#include "document.h"
#include "fake_backend.h"
#include "gtest/gtest.h"

TEST(code_element, inline_code_element) {
    fake_backend fbcknd;

    // reset some preset stylings to ease testing
    auto& ss = fbcknd.get_style_sheet();
    ss.set_float_param("margin_top", 0.f);
    ss.set_float_param("margin_bottom", 0.f);
    ss.set_float_param("document.margin", 0.f);
    ss.set_float_param("image.margin_left", 2.f);
    ss.set_float_param("image.margin_right", 2.f);
    ss.set_float_param("code.margin_left", 5.f);
    ss.set_float_param("code.margin_right", 5.f);
    ss.set_string_param("code.font", "test_code_font");
    ss.set_string_param("code.background_color", "#aabbccdd");

    auto doc = dcmr::document::from_string("Hello `code`");
    doc.set_backend(&fbcknd);
    doc.layout(500);
    doc.render(dcmr::vec2(0, 0), 500);

    ASSERT_EQ(2, fbcknd.m_draw_string_calls.size());

    // the second string must use the code font
    EXPECT_STREQ("test_code_font",
                 fbcknd.m_draw_string_calls[1].m_font.m_family.c_str());

    // both words must be rendered on the same base line
    EXPECT_EQ(fbcknd.m_draw_string_calls[0].m_pos.y(),
              fbcknd.m_draw_string_calls[1].m_pos.y());

    EXPECT_EQ(65.f, fbcknd.m_draw_string_calls[1].m_pos.x());

    // test if the background rectangle is positioned correctly
    EXPECT_EQ(1, fbcknd.m_draw_rect_calls.size());

    EXPECT_EQ(dcmr::vec2(60.0, 0.0),
              fbcknd.m_draw_rect_calls[0].m_rect.top_left());
    // the rectangle must include the 5+5px margin
    EXPECT_EQ(dcmr::vec2(110.0, 16.0),
              fbcknd.m_draw_rect_calls[0].m_rect.bottom_right());
}

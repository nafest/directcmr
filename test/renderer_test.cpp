#include "gtest/gtest.h"

#include "document.h"
#include "fake_renderer.h"

TEST(Renderer, RenderSimple) {
    fake_renderer rndr;
    rndr.set_float_param("list_margin", 5.f);
    document d = document::fromString("Hello *world*");
    d.set_renderer(&rndr);
    d.layout(100);
    d.render(vec2(0, 0), 100);

    EXPECT_EQ(2, rndr.m_draw_string_calls.size());
    EXPECT_EQ(0, rndr.m_draw_string_calls[0].m_pos.x());
    EXPECT_STREQ("Normal", rndr.m_draw_string_calls[0].m_font.m_style.c_str());
    EXPECT_EQ(60, rndr.m_draw_string_calls[1].m_pos.x());
    EXPECT_STREQ("Italic", rndr.m_draw_string_calls[1].m_font.m_style.c_str());
}

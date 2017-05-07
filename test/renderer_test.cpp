#include "gtest/gtest.h"

#include "document.h"
#include "fake_renderer.h"

class renderer_test : public ::testing::Test {
  public:
    renderer_test() : ::testing::Test() {}

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

TEST_F(renderer_test, string_param_fallback) {
    frndr.set_string_param("bar", "42");

    EXPECT_STREQ("42", frndr.get_string_param("foo.bar").c_str());
}

TEST_F(renderer_test, float_param_fallback) {
    frndr.set_float_param("bar", 42.f);

    EXPECT_EQ(42.f, frndr.get_float_param("foo.bar"));
}

TEST_F(renderer_test, RenderSimple) {
    frndr.set_float_param("list.margin_left", 5.f);
    auto d = cmr::document::from_string("Hello *world*");
    d.set_renderer(&frndr);
    d.layout(200);
    d.render(cmr::vec2(0, 0), 100);

    EXPECT_EQ(2, frndr.m_draw_string_calls.size());
    EXPECT_EQ(0, frndr.m_draw_string_calls[0].m_pos.x());
    EXPECT_STREQ("Normal", frndr.m_draw_string_calls[0].m_font.m_style.c_str());
    EXPECT_EQ(60, frndr.m_draw_string_calls[1].m_pos.x());
    EXPECT_STREQ("Italic", frndr.m_draw_string_calls[1].m_font.m_style.c_str());
}

TEST_F(renderer_test, RenderList) {
    frndr.set_float_param("list.margin_left", 5.f);
    auto d = cmr::document::from_string("- Item1\n- Item2");
    d.set_renderer(&frndr);
    d.layout(100);
    d.render(cmr::vec2(0, 0), 100);

    auto fnt = d.get_root_element()->get_font(&frndr);

    EXPECT_EQ(2, frndr.m_draw_marker_calls.size());
    auto mrect = frndr.m_draw_marker_calls[0].m_marker_rect;
    EXPECT_EQ(cmr::vec2(0.f, 0.f), mrect.top_left());
    EXPECT_EQ(cmr::vec2(5.f, fnt->get_line_height()), mrect.bottom_right());

    auto mrect2 = frndr.m_draw_marker_calls[1].m_marker_rect;
    EXPECT_EQ(cmr::vec2(0.f, fnt->get_line_height()), mrect2.top_left());
    EXPECT_EQ(cmr::vec2(5.f, 2 * fnt->get_line_height()),
              mrect2.bottom_right());
}

TEST_F(renderer_test, RenderCodeSpan) {
    frndr.set_float_param("code.margin_left", 5.f);
    frndr.set_float_param("code.margin_right", 6.f);
    auto d = cmr::document::from_string("foo `void main` bar");

    // the font width of normal text is 10px per character, while
    // it is 12px for code spans.
    // hence the strings should start at:
    // 3*10+10 4*12+12 4*12+10
    // 0 40 100 158

    d.set_renderer(&frndr);
    d.layout(500);
    d.render(cmr::vec2(0, 0), 500);

    EXPECT_EQ(4, frndr.m_draw_string_calls.size());
    EXPECT_EQ(0, frndr.m_draw_string_calls[0].m_pos.x());
    EXPECT_STREQ("foo", frndr.m_draw_string_calls[0].m_text.c_str());
    EXPECT_EQ(45, frndr.m_draw_string_calls[1].m_pos.x());
    EXPECT_EQ(105, frndr.m_draw_string_calls[2].m_pos.x());
    EXPECT_EQ(169, frndr.m_draw_string_calls[3].m_pos.x());
}

TEST(get_margin, nothing_set) {
    fake_renderer rndr;
    EXPECT_EQ(0.f, rndr.get_margin("foo_elem").left);
}

TEST(get_margin, specific_set) {
    fake_renderer rndr;
    rndr.set_float_param("margin", 45.f);
    rndr.set_float_param("margin_left", 44.f);
    rndr.set_float_param("foo_elem.margin", 43.f);
    rndr.set_float_param("foo_elem.margin_left", 42.f);

    EXPECT_EQ(42.f, rndr.get_margin("foo_elem").left);
}

TEST(get_margin, specific_elem_set) {
    fake_renderer rndr;
    rndr.set_float_param("margin", 45.f);
    rndr.set_float_param("margin_left", 44.f);
    rndr.set_float_param("foo_elem.margin", 43.f);

    EXPECT_EQ(43.f, rndr.get_margin("foo_elem").left);
}

TEST(get_margin, specific_general_set) {
    fake_renderer rndr;
    rndr.set_float_param("margin", 45.f);
    rndr.set_float_param("margin_left", 44.f);

    EXPECT_EQ(44.f, rndr.get_margin("foo_elem").left);
}

TEST(get_margin, general_set) {
    fake_renderer rndr;
    rndr.set_float_param("margin", 45.f);

    EXPECT_EQ(45.f, rndr.get_margin("foo_elem").left);
}
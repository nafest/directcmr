#include "gtest/gtest.h"

#include "document.h"
#include "fake_backend.h"

class backend_test : public ::testing::Test {
  public:
    backend_test() : ::testing::Test() {}

  protected:
    virtual void SetUp() override {
        // reset some preset stylings to ease testing
        fbcknd.set_float_param("margin_top", 0.f);
        fbcknd.set_float_param("margin_bottom", 0.f);
        fbcknd.set_float_param("document.margin", 0.f);
    }

    virtual void TearDown() override {}

    fake_backend fbcknd;
};

TEST_F(backend_test, string_param_fallback) {
    fbcknd.set_string_param("bar", "42");

    EXPECT_STREQ("42", fbcknd.get_string_param("foo.bar").c_str());
}

TEST_F(backend_test, float_param_fallback) {
    fbcknd.set_float_param("bar", 42.f);

    EXPECT_EQ(42.f, fbcknd.get_float_param("foo.bar"));
}

TEST_F(backend_test, RenderSimple) {
    fbcknd.set_float_param("list.margin_left", 5.f);
    auto d = cmr::document::from_string("Hello *world*");
    d.set_backend(&fbcknd);
    d.layout(200);
    d.render(cmr::vec2(0, 0), 100);

    EXPECT_EQ(2, fbcknd.m_draw_string_calls.size());
    EXPECT_EQ(0, fbcknd.m_draw_string_calls[0].m_pos.x());
    EXPECT_STREQ("Normal", fbcknd.m_draw_string_calls[0].m_font.m_style.c_str());
    EXPECT_EQ(60, fbcknd.m_draw_string_calls[1].m_pos.x());
    EXPECT_STREQ("Italic", fbcknd.m_draw_string_calls[1].m_font.m_style.c_str());
}

TEST_F(backend_test, RenderList) {
    fbcknd.set_float_param("list.margin_left", 5.f);
    auto d = cmr::document::from_string("- Item1\n- Item2");
    d.set_backend(&fbcknd);
    d.layout(100);
    d.render(cmr::vec2(0, 0), 100);

    auto fnt = d.get_root_element()->get_font(&fbcknd);

    EXPECT_EQ(2, fbcknd.m_draw_marker_calls.size());
    auto mrect = fbcknd.m_draw_marker_calls[0].m_marker_rect;
    EXPECT_EQ(cmr::vec2(0.f, 0.f), mrect.top_left());
    EXPECT_EQ(cmr::vec2(5.f, fnt->get_line_height()), mrect.bottom_right());

    auto mrect2 = fbcknd.m_draw_marker_calls[1].m_marker_rect;
    EXPECT_EQ(cmr::vec2(0.f, fnt->get_line_height()), mrect2.top_left());
    EXPECT_EQ(cmr::vec2(5.f, 2 * fnt->get_line_height()),
              mrect2.bottom_right());
}

TEST_F(backend_test, RenderCodeSpan) {
    fbcknd.set_float_param("code.margin_left", 5.f);
    fbcknd.set_float_param("code.margin_right", 6.f);
    auto d = cmr::document::from_string("foo `void main` bar");

    // the font width of normal text is 10px per character, while
    // it is 12px for code spans.
    // hence the strings should start at:
    // 3*10+10 4*12+12 4*12+10
    // 0 40 100 158

    d.set_backend(&fbcknd);
    d.layout(500);
    d.render(cmr::vec2(0, 0), 500);

    EXPECT_EQ(4, fbcknd.m_draw_string_calls.size());
    EXPECT_EQ(0, fbcknd.m_draw_string_calls[0].m_pos.x());
    EXPECT_STREQ("foo", fbcknd.m_draw_string_calls[0].m_text.c_str());
    EXPECT_EQ(45, fbcknd.m_draw_string_calls[1].m_pos.x());
    EXPECT_EQ(105, fbcknd.m_draw_string_calls[2].m_pos.x());
    EXPECT_EQ(169, fbcknd.m_draw_string_calls[3].m_pos.x());
}

TEST(get_margin, nothing_set) {
    fake_backend fbcknd;
    EXPECT_EQ(0.f, fbcknd.get_margin("foo_elem").left);
}

TEST(get_margin, specific_set) {
    fake_backend fbcknd;
    fbcknd.set_float_param("margin", 45.f);
    fbcknd.set_float_param("margin_left", 44.f);
    fbcknd.set_float_param("foo_elem.margin", 43.f);
    fbcknd.set_float_param("foo_elem.margin_left", 42.f);

    EXPECT_EQ(42.f, fbcknd.get_margin("foo_elem").left);
}

TEST(get_margin, specific_elem_set) {
    fake_backend fbcknd;
    fbcknd.set_float_param("margin", 45.f);
    fbcknd.set_float_param("margin_left", 44.f);
    fbcknd.set_float_param("foo_elem.margin", 43.f);

    EXPECT_EQ(43.f, fbcknd.get_margin("foo_elem").left);
}

TEST(get_margin, specific_general_set) {
    fake_backend fbcknd;
    fbcknd.set_float_param("margin", 45.f);
    fbcknd.set_float_param("margin_left", 44.f);

    EXPECT_EQ(44.f, fbcknd.get_margin("foo_elem").left);
}

TEST(get_margin, general_set) {
    fake_backend fbcknd;
    fbcknd.set_float_param("margin", 45.f);

    EXPECT_EQ(45.f, fbcknd.get_margin("foo_elem").left);
}
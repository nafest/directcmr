#include "document.h"
#include "fake_renderer.h"
#include "gtest/gtest.h"

class linebreak_element_test : public ::testing::Test {
  public:
    linebreak_element_test() : ::testing::Test() {}

  protected:
    virtual void SetUp() override {
        // reset some preset stylings to ease testing
        frndr.set_float_param("margin_top", 0.f);
        frndr.set_float_param("margin_bottom", 0.f);
        frndr.set_float_param("document.margin", 0.f);
        frndr.set_float_param("image.margin_left", 2.f);
        frndr.set_float_param("image.margin_right", 2.f);
    }

    virtual void TearDown() override {}

    fake_renderer frndr;
};

TEST_F(linebreak_element_test, paragraph_with_linebreak) {
    document doc = document::from_string("hello  \nworld");
    doc.set_renderer(&frndr);
    doc.layout(500);
    doc.render(vec2(), 500);

    ASSERT_EQ(2, frndr.m_draw_string_calls.size());
    EXPECT_EQ(vec2(0.f, 12.f), frndr.m_draw_string_calls[0].m_pos);
    EXPECT_EQ(vec2(0.f, 28.f), frndr.m_draw_string_calls[1].m_pos);
}
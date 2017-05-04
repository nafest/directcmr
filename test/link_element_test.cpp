#include "document.h"
#include "fake_renderer.h"
#include "gtest/gtest.h"

class link_element_test : public ::testing::Test {
  public:
    link_element_test() : ::testing::Test() {}

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

TEST_F(link_element_test, simple_link) {
    frndr.set_string_param("link.color", "#0000ffff");

    document doc = document::from_string("[hello link](/uri)");
    doc.set_renderer(&frndr);
    doc.layout(500);
    doc.render(vec2(), 500);

    ASSERT_EQ(2, frndr.m_draw_string_calls.size());
    EXPECT_EQ(color("#0000ffff"), frndr.m_draw_string_calls[0].m_color);
}

TEST_F(link_element_test, visited_link) {
    frndr.set_string_param("link.color", "#0000ffff");
    frndr.set_string_param("link.visited_color", "#ff0000ff");
    frndr.add_visited_link("/uri");

    document doc = document::from_string("[hello link](/uri)");
    doc.set_renderer(&frndr);
    doc.layout(500);
    doc.render(vec2(), 500);

    ASSERT_EQ(2, frndr.m_draw_string_calls.size());
    EXPECT_EQ(color("#ff0000ff"), frndr.m_draw_string_calls[0].m_color);
}


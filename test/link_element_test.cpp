#include "document.h"
#include "fake_backend.h"
#include "gtest/gtest.h"

class link_element_test : public ::testing::Test {
  public:
    link_element_test() : ::testing::Test() {}

  protected:
    virtual void SetUp() override {
        // reset some preset stylings to ease testing
        auto ss = fbcknd.get_style_sheet();
        ss.set_float_param("margin_top", 0.f);
        ss.set_float_param("margin_bottom", 0.f);
        ss.set_float_param("document.margin", 0.f);
        ss.set_float_param("image.margin_left", 2.f);
        ss.set_float_param("image.margin_right", 2.f);
    }

    virtual void TearDown() override {}

    fake_backend fbcknd;
};

TEST_F(link_element_test, simple_link) {
    fbcknd.get_style_sheet().set_string_param("link.color", "#0000ffff");

    auto doc = dcmr::document::from_string("[hello link](/uri)");
    doc.set_backend(&fbcknd);
    doc.layout(500);
    doc.render(dcmr::vec2(), 500);

    ASSERT_EQ(2, fbcknd.m_draw_string_calls.size());
    EXPECT_EQ(dcmr::color("#0000ffff"), fbcknd.m_draw_string_calls[0].m_color);
}

TEST_F(link_element_test, visited_link) {
    auto& ss = fbcknd.get_style_sheet();
    ss.set_string_param("link.color", "#0000ffff");
    ss.set_string_param("link.visited_color", "#ff0000ff");
    fbcknd.add_visited_link("/uri");

    auto doc = dcmr::document::from_string("[hello link](/uri)");
    doc.set_backend(&fbcknd);
    doc.layout(500);
    doc.render(dcmr::vec2(), 500);

    ASSERT_EQ(2, fbcknd.m_draw_string_calls.size());
    EXPECT_EQ(dcmr::color("#ff0000ff"), fbcknd.m_draw_string_calls[0].m_color);
}

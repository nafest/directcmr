#include "document.h"
#include "fake_backend.h"
#include "gtest/gtest.h"

class linebreak_element_test : public ::testing::Test {
  public:
    linebreak_element_test() : ::testing::Test() {}

  protected:
    virtual void SetUp() override {
        // reset some preset stylings to ease testing
        auto& ss = fbcknd.get_style_sheet();
        ss.set_float_param("margin_top", 0.f);
        ss.set_float_param("margin_bottom", 0.f);
        ss.set_float_param("document.margin", 0.f);
        ss.set_float_param("image.margin_left", 2.f);
        ss.set_float_param("image.margin_right", 2.f);
    }

    virtual void TearDown() override {}

    fake_backend fbcknd;
};

TEST_F(linebreak_element_test, paragraph_with_linebreak) {
    auto doc = dcmr::document::from_string("hello  \nworld");
    doc.set_backend(&fbcknd);
    doc.layout(500);
    doc.render(dcmr::vec2(), 500);

    ASSERT_EQ(2, fbcknd.m_draw_string_calls.size());
    EXPECT_EQ(dcmr::vec2(0.f, 12.f), fbcknd.m_draw_string_calls[0].m_pos);
    EXPECT_EQ(dcmr::vec2(0.f, 28.f), fbcknd.m_draw_string_calls[1].m_pos);
}
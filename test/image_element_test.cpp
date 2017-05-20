#include "document.h"
#include "fake_backend.h"
#include "gtest/gtest.h"

/* Test for cases of embedding an image in a paragraph
 *   - image slightly larger than font ascent
 *   - image smaller than font height
 *   - image larger than what is left on the current line, but smaller
 *     then the maximum paragraph width
 *   - image wider than the maximum paragraph width */

class image_element_test : public ::testing::Test {
  public:
    image_element_test() : ::testing::Test() {}

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

TEST_F(image_element_test, paragraph_with_small_image) {
    fbcknd.set_image_extents("image.png", 50, 40);
    auto doc = dcmr::document::from_string("Hello ![](image.png) image", false);

    doc.set_backend(&fbcknd);
    doc.layout(500);
    doc.render(dcmr::vec2(0, 0), 500);

    EXPECT_EQ(1, fbcknd.m_draw_image_calls.size());
    EXPECT_EQ(50, fbcknd.m_draw_image_calls[0].m_width);
    EXPECT_EQ(40, fbcknd.m_draw_image_calls[0].m_height);
    // there should be a space at the left and at the right of the image
    EXPECT_EQ(62.0, fbcknd.m_draw_image_calls[0].m_pos.x());
    EXPECT_EQ(0.0, fbcknd.m_draw_image_calls[0].m_pos.y());

    // the base line should be at 40.0
    EXPECT_EQ(0, fbcknd.m_draw_string_calls[0].m_pos.x());
    EXPECT_EQ(40.0, fbcknd.m_draw_string_calls[0].m_pos.y());

    // 62.0 + 50 (image_width) + 2.0 margin + 10 extra space
    EXPECT_EQ(124.0, fbcknd.m_draw_string_calls[1].m_pos.x());
    EXPECT_EQ(40.0, fbcknd.m_draw_string_calls[1].m_pos.y());
}

TEST_F(image_element_test, paragraph_with_tiny_image) {
    fbcknd.set_image_extents("image.png", 50, 5);
    dcmr::document doc = dcmr::document::from_string("Hello ![](image.png) image", false);

    doc.set_backend(&fbcknd);
    doc.layout(500);
    doc.render(dcmr::vec2(0, 0), 500);

    EXPECT_EQ(1, fbcknd.m_draw_image_calls.size());
    EXPECT_EQ(50, fbcknd.m_draw_image_calls[0].m_width);
    EXPECT_EQ(5, fbcknd.m_draw_image_calls[0].m_height);
    // there should be a space at the left and at the right of the image
    EXPECT_EQ(62.0, fbcknd.m_draw_image_calls[0].m_pos.x());
    EXPECT_EQ(7.0, fbcknd.m_draw_image_calls[0].m_pos.y());

    // the base line should be at 12.0
    EXPECT_EQ(0, fbcknd.m_draw_string_calls[0].m_pos.x());
    EXPECT_EQ(12.0, fbcknd.m_draw_string_calls[0].m_pos.y());

    // 62.0 + 50 (image_width) + 2.0 margin + 10 extra space
    EXPECT_EQ(124.0, fbcknd.m_draw_string_calls[1].m_pos.x());
    EXPECT_EQ(12.0, fbcknd.m_draw_string_calls[1].m_pos.y());
}

// add an image such that it is advanced to the next line
// with the width beeing below the paragaph width
TEST_F(image_element_test, paragraph_with_larger_image) {
    fbcknd.set_image_extents("image.png", 400, 50);
    dcmr::document doc = dcmr::document::from_string(
        "HelloWorldHelloWorld ![](image.png) image", false);

    doc.set_backend(&fbcknd);
    doc.layout(500);
    doc.render(dcmr::vec2(0, 0), 500);

    EXPECT_EQ(1, fbcknd.m_draw_image_calls.size());
    EXPECT_EQ(400, fbcknd.m_draw_image_calls[0].m_width);
    EXPECT_EQ(50, fbcknd.m_draw_image_calls[0].m_height);

    // there should be a space at the left and at the right of the image
    EXPECT_EQ(2.0, fbcknd.m_draw_image_calls[0].m_pos.x());
    EXPECT_EQ(16.0, fbcknd.m_draw_image_calls[0].m_pos.y());

    EXPECT_EQ(0, fbcknd.m_draw_string_calls[0].m_pos.x());
    EXPECT_EQ(12.0, fbcknd.m_draw_string_calls[0].m_pos.y());

    // 2.0 margin + 400 (image_width) + 2.0 margin + 10 extra space
    EXPECT_EQ(414.0, fbcknd.m_draw_string_calls[1].m_pos.x());

    // the base line of the second string should match the bottom
    // of the image
    EXPECT_EQ(16.0 + 50.0, fbcknd.m_draw_string_calls[1].m_pos.y());
}

// add an image such that it is advanced to the next line
// with the width beeing larger than the paragaph width
TEST_F(image_element_test, paragraph_with_very_large_image) {
    fbcknd.set_image_extents("image.png", 992, 50);
    dcmr::document doc = dcmr::document::from_string(
        "HelloWorldHelloWorld ![](image.png) image", false);

    doc.set_backend(&fbcknd);
    doc.layout(500);
    doc.render(dcmr::vec2(0, 0), 500);

    EXPECT_EQ(1, fbcknd.m_draw_image_calls.size());
    EXPECT_EQ(496, fbcknd.m_draw_image_calls[0].m_width);
    EXPECT_EQ(25, fbcknd.m_draw_image_calls[0].m_height);

    // there should be a space at the left and at the right of the image
    EXPECT_EQ(2.0, fbcknd.m_draw_image_calls[0].m_pos.x());
    EXPECT_EQ(16.0, fbcknd.m_draw_image_calls[0].m_pos.y());

    EXPECT_EQ(0, fbcknd.m_draw_string_calls[0].m_pos.x());
    EXPECT_EQ(12.0, fbcknd.m_draw_string_calls[0].m_pos.y());

    // 2.0 margin + 400 (image_width) + 2.0 margin + 10 extra space
    EXPECT_EQ(0, fbcknd.m_draw_string_calls[1].m_pos.x());

    // the string should be rendered below the image
    // 12 + 16 + (25-12) + 16
    EXPECT_EQ(16.0 + 25.0 + 16.0, fbcknd.m_draw_string_calls[1].m_pos.y());
}
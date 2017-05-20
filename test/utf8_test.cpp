#include "document.h"
#include "fake_backend.h"
#include "gtest/gtest.h"

TEST(utf8_test, calls_backend_correctly) {
    fake_backend fbcknd;
    auto doc = dcmr::document::from_string(u8"äöü öäü");

    doc.set_backend(&fbcknd);
    doc.layout(500);
    doc.render(dcmr::vec2(), 500);

    ASSERT_EQ(2, fbcknd.m_draw_string_calls.size());
    EXPECT_STREQ(u8"äöü", fbcknd.m_draw_string_calls[0].m_text.c_str());
    EXPECT_STREQ(u8"öäü", fbcknd.m_draw_string_calls[1].m_text.c_str());
}
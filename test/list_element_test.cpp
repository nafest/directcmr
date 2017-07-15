#include "document.h"
#include "fake_backend.h"
#include "list_element.h"
#include "gtest/gtest.h"

// the paragraph margin must be taken into
// for every list item
TEST(list_element, pays_respect_to_paragraph_margin) {
    fake_backend fbcknd;
    auto &ss = fbcknd.get_style_sheet();
    ss.set_float_param("margin_top", 2.f);
    ss.set_float_param("margin_bottom", 2.f);
    ss.set_float_param("document.margin", 0.f);
    ss.set_float_param("table_cell.margin", 2.0);
    ss.set_float_param("paragraph.margin_top", 4.f);
    ss.set_float_param("paragraph.margin_bottom", 4.f);
    ss.set_float_param("list.margin_left", 5.f);
    ss.set_float_param("item.margin_top", 0.f);

    auto doc = dcmr::document::from_string("1. Item1\n2. Item2");

    doc.set_backend(&fbcknd);
    auto height = doc.layout(500);
    doc.render(dcmr::vec2(), 500);

    auto ascent = doc.get_root_element()->get_font(&fbcknd)->get_ascent();

    EXPECT_EQ(4, fbcknd.m_draw_string_calls.size());
    EXPECT_EQ(ascent + 6, fbcknd.m_draw_string_calls[0].m_pos.y());
    EXPECT_EQ(ascent + 6, fbcknd.m_draw_string_calls[1].m_pos.y());
}

TEST(list_element, pays_respect_to_item_margin) {
    fake_backend fbcknd;
    auto &ss = fbcknd.get_style_sheet();
    ss.set_float_param("margin_top", 2.f);
    ss.set_float_param("margin_bottom", 2.f);
    ss.set_float_param("document.margin", 0.f);
    ss.set_float_param("table_cell.margin", 2.0);
    ss.set_float_param("paragraph.margin_top", 4.f);
    ss.set_float_param("paragraph.margin_bottom", 4.f);
    ss.set_float_param("list.margin_left", 5.f);
    ss.set_float_param("item.margin_top", 3.f);

    auto doc = dcmr::document::from_string("1. Item1\n2. Item2");

    doc.set_backend(&fbcknd);
    auto height = doc.layout(500);
    doc.render(dcmr::vec2(), 500);

    auto ascent = doc.get_root_element()->get_font(&fbcknd)->get_ascent();

    EXPECT_EQ(4, fbcknd.m_draw_string_calls.size());
    EXPECT_EQ(ascent + 6 + 3, fbcknd.m_draw_string_calls[0].m_pos.y());
    EXPECT_EQ(ascent + 6 + 3, fbcknd.m_draw_string_calls[1].m_pos.y());
}

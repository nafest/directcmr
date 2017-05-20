#include "document.h"
#include "fake_backend.h"
#include "table_element.h"
#include "gtest/gtest.h"

TEST(distribute_width, sufficient_space) {
    std::vector<float> min_widths{20.0, 20.0, 20.0};
    auto widths = dcmr::table_element::distribute_width(300, 3, min_widths);

    EXPECT_EQ(100, widths[0]);
    EXPECT_EQ(100, widths[1]);
    EXPECT_EQ(100, widths[2]);
}

TEST(distribute_width, pay_respect_to_min_width) {
    std::vector<float> min_widths{30.0, 120.0, 30.0};
    auto widths = dcmr::table_element::distribute_width(300, 3, min_widths);

    EXPECT_EQ(70, widths[0]);
    EXPECT_EQ(160, widths[1]);
    EXPECT_EQ(70, widths[2]);
}

TEST(distribute_width, pay_respect_to_min_width_difficult) {
    std::vector<float> min_widths{150.0, 90.0, 30.0};
    auto widths = dcmr::table_element::distribute_width(300, 3, min_widths);

    EXPECT_EQ(160, widths[0]);
    EXPECT_EQ(100, widths[1]);
    EXPECT_EQ(40, widths[2]);
}

TEST(table_cell_preferred_width, includes_margin) {
    fake_backend fbcknd;
    auto &ss = fbcknd.get_style_sheet();
    ss.set_float_param("table_cell.margin_left", 12.0);
    ss.set_float_param("table_cell.margin_right", 13.0);

    dcmr::document doc = dcmr::document::from_string(
        "| lorem | ipsum |\n| --- | --- |\n| lorem | ipsum dolor |");
    // table -> table_row -> table_cell
    auto table = doc.get_root_element()->children()[0];
    auto table_row =
        table->children()[1]; // select the second row, not the header
    auto table_cell = table_row->children()[0];
    EXPECT_EQ(50.f + 12.f + 13.f, table_cell->preferred_width(&fbcknd));

    table_cell = table_row->children()[1];
    EXPECT_EQ(110.f + 12.f + 13.f, table_cell->preferred_width(&fbcknd));
}

TEST(table_cell_min_width, includes_margin) {
    fake_backend fbcknd;
    auto &ss = fbcknd.get_style_sheet();
    ss.set_float_param("table_cell.margin_left", 12.0);
    ss.set_float_param("table_cell.margin_right", 13.0);

    dcmr::document doc = dcmr::document::from_string(
        "| lorem | ipsum |\n| --- | --- |\n| lorem | ipsum dolores |");
    // table -> table_row -> table_cell
    auto table = doc.get_root_element()->children()[0];
    auto table_row =
        table->children()[1]; // select the second row, not the header
    auto table_cell = table_row->children()[0];
    EXPECT_EQ(50.f + 12.f + 13.f, table_cell->min_width(&fbcknd));

    table_cell = table_row->children()[1];
    EXPECT_EQ(70.f + 12.f + 13.f, table_cell->min_width(&fbcknd));
}

TEST(table_cell_preferred_width, renders_correctly) {
    fake_backend fbcknd;
    auto &ss = fbcknd.get_style_sheet();
    ss.set_float_param("margin_top", 0.f);
    ss.set_float_param("margin_bottom", 0.f);
    ss.set_float_param("document.margin", 0.f);
    ss.set_float_param("table_cell.margin", 2.0);

    auto doc = dcmr::document::from_string(
        "| lorem | ipsum |\n| --- | --- |\n| lorem | ipsum dolor |");

    doc.set_backend(&fbcknd);
    auto height = doc.layout(500);
    doc.render(dcmr::vec2(), 500);

    auto table = doc.get_root_element()->children()[0];
    auto fnt = table->get_font(&fbcknd);

    ASSERT_EQ(5, fbcknd.m_draw_string_calls.size());
    // lorem
    EXPECT_EQ(3, fbcknd.m_draw_string_calls[0].m_pos.x());
    EXPECT_EQ(1 + 2 + fnt->get_ascent(),
              fbcknd.m_draw_string_calls[0].m_pos.y());
    // ipsum
    EXPECT_EQ(3 + 50 + 5, fbcknd.m_draw_string_calls[1].m_pos.x());
    EXPECT_EQ(1 + 2 + fnt->get_ascent(),
              fbcknd.m_draw_string_calls[1].m_pos.y());
    // lorem
    EXPECT_EQ(3, fbcknd.m_draw_string_calls[2].m_pos.x());
    EXPECT_EQ(1 + 2 + fnt->get_line_height() + 2 + 1 + 2 + fnt->get_ascent(),
              fbcknd.m_draw_string_calls[2].m_pos.y());

    ASSERT_EQ(6, fbcknd.m_draw_line_calls.size());

    // the horizontal lines are rendered first
    EXPECT_EQ(dcmr::vec2(0.f, 0.5f), fbcknd.m_draw_line_calls[0].m_from);
    // 3 + 50 + 5 + 110 + 3 = 171
    EXPECT_EQ(dcmr::vec2(171.f, 0.5f), fbcknd.m_draw_line_calls[0].m_to);

    auto second_line = 3 + fnt->get_line_height() + 2 + 0.5f;
    EXPECT_EQ(dcmr::vec2(0.f, second_line), fbcknd.m_draw_line_calls[1].m_from);
    EXPECT_EQ(dcmr::vec2(171.f, second_line), fbcknd.m_draw_line_calls[1].m_to);

    auto third_line =
        3 + fnt->get_line_height() + 5 + fnt->get_line_height() + 2 + 0.5f;
    EXPECT_EQ(dcmr::vec2(0.f, third_line), fbcknd.m_draw_line_calls[2].m_from);
    EXPECT_EQ(dcmr::vec2(171.f, third_line), fbcknd.m_draw_line_calls[2].m_to);

    // now the vertical lines
    EXPECT_EQ(dcmr::vec2(0.5f, 0.f), fbcknd.m_draw_line_calls[3].m_from);
    EXPECT_EQ(dcmr::vec2(0.5f, third_line), fbcknd.m_draw_line_calls[3].m_to);

    EXPECT_EQ(dcmr::vec2(55.5f, 0.f), fbcknd.m_draw_line_calls[4].m_from);
    EXPECT_EQ(dcmr::vec2(55.5f, third_line), fbcknd.m_draw_line_calls[4].m_to);

    EXPECT_EQ(dcmr::vec2(170.5f, 0.f), fbcknd.m_draw_line_calls[5].m_from);
    EXPECT_EQ(dcmr::vec2(170.5f, third_line), fbcknd.m_draw_line_calls[5].m_to);
}

// add a test where the preferred with is too large
TEST(table_cell_preferred_width, renders_too_wide_table_correctly) {
    fake_backend fbcknd;
    auto &ss = fbcknd.get_style_sheet();
    ss.set_float_param("margin_top", 0.f);
    ss.set_float_param("margin_bottom", 0.f);
    ss.set_float_param("document.margin", 0.f);
    ss.set_float_param("table_cell.margin", 2.0);

    auto doc = dcmr::document::from_string(
        "| lorem | ipsum |\n| --- | --- |\n| lorem | ipsum dolor |");

    doc.set_backend(&fbcknd);
    auto height = doc.layout(120);
    doc.render(dcmr::vec2(), 500);

    auto table = doc.get_root_element()->children()[0];
    auto fnt = table->get_font(&fbcknd);

    ASSERT_EQ(5, fbcknd.m_draw_string_calls.size());
    // lorem
    EXPECT_EQ(3, fbcknd.m_draw_string_calls[0].m_pos.x());
    EXPECT_EQ(1 + 2 + fnt->get_ascent(),
              fbcknd.m_draw_string_calls[0].m_pos.y());
    // ipsum
    // the minimum width of both cells should be 50 + 4
    // i.e. 120 - 2*54 - 3 = 9 px
    // are equally distributed, e.g. 4.5 px per cell
    EXPECT_EQ(62.5, fbcknd.m_draw_string_calls[1].m_pos.x());
    EXPECT_EQ(1 + 2 + fnt->get_ascent(),
              fbcknd.m_draw_string_calls[1].m_pos.y());
    // lorem
    EXPECT_EQ(3, fbcknd.m_draw_string_calls[2].m_pos.x());

    EXPECT_EQ(1 + 2 + fnt->get_line_height() + 2 + 1 + 2 + fnt->get_ascent(),
              fbcknd.m_draw_string_calls[2].m_pos.y());

    ASSERT_EQ(6, fbcknd.m_draw_line_calls.size());

    // the horizontal lines are rendered first
    EXPECT_EQ(dcmr::vec2(0.f, 0.5f), fbcknd.m_draw_line_calls[0].m_from);
    // 3 + 50 + 5 + 110 + 3 = 171
    EXPECT_EQ(dcmr::vec2(120.f, 0.5f), fbcknd.m_draw_line_calls[0].m_to);

    auto second_line = 3 + fnt->get_line_height() + 2 + 0.5f;
    EXPECT_EQ(dcmr::vec2(0.f, second_line), fbcknd.m_draw_line_calls[1].m_from);
    EXPECT_EQ(dcmr::vec2(120.f, second_line), fbcknd.m_draw_line_calls[1].m_to);

    // the second row now spans over two lines
    auto third_line =
        second_line + 0.5f + 2 + 2 * fnt->get_line_height() + 2 + 0.5f;
    EXPECT_EQ(dcmr::vec2(0.f, third_line), fbcknd.m_draw_line_calls[2].m_from);
    EXPECT_EQ(dcmr::vec2(120.f, third_line), fbcknd.m_draw_line_calls[2].m_to);

    // now the vertical lines
    EXPECT_EQ(dcmr::vec2(0.5f, 0.f), fbcknd.m_draw_line_calls[3].m_from);
    EXPECT_EQ(dcmr::vec2(0.5f, third_line), fbcknd.m_draw_line_calls[3].m_to);

    EXPECT_EQ(dcmr::vec2(60.0f, 0.f), fbcknd.m_draw_line_calls[4].m_from);
    EXPECT_EQ(dcmr::vec2(60.0f, third_line), fbcknd.m_draw_line_calls[4].m_to);

    EXPECT_EQ(dcmr::vec2(119.5f, 0.f), fbcknd.m_draw_line_calls[5].m_from);
    EXPECT_EQ(dcmr::vec2(119.5f, third_line), fbcknd.m_draw_line_calls[5].m_to);
}

#include "document.h"
#include "table_element.h"
#include "fake_renderer.h"
#include "gtest/gtest.h"


TEST(distribute_width, sufficient_space) {
    std::vector<float> min_widths {20.0, 20.0, 20.0};
    auto widths = table_element::distribute_width(300, 3, min_widths);

    EXPECT_EQ(100, widths[0]);
    EXPECT_EQ(100, widths[1]);
    EXPECT_EQ(100, widths[2]);
}

TEST(distribute_width, pay_respect_to_min_width) {
    std::vector<float> min_widths {30.0, 120.0, 30.0};
    auto widths = table_element::distribute_width(300, 3, min_widths);

    EXPECT_EQ(70, widths[0]);
    EXPECT_EQ(160, widths[1]);
    EXPECT_EQ(70, widths[2]);
}

TEST(distribute_width, pay_respect_to_min_width_difficult) {
    std::vector<float> min_widths {150.0, 90.0, 30.0};
    auto widths = table_element::distribute_width(300, 3, min_widths);

    EXPECT_EQ(160, widths[0]);
    EXPECT_EQ(100, widths[1]);
    EXPECT_EQ(40, widths[2]);
}

TEST(table_cell_preferred_width, includes_margin) {
    fake_renderer  frndr;
    frndr.set_float_param("table_cell.margin_left", 12.0);
    frndr.set_float_param("table_cell.margin_right", 13.0);

    document doc = document::from_string("| lorem | ipsum |\n| --- | --- |\n| lorem | ipsum dolor |");
    // table -> table_row -> table_cell
    auto table = doc.get_root_element()->children()[0];
    auto table_row = table->children()[1]; // select the second row, not the header
    auto table_cell = table_row->children()[0];
    EXPECT_EQ(50.f + 12.f + 13.f, table_cell->preferred_width(&frndr));

    table_cell = table_row->children()[1];
    EXPECT_EQ(110.f + 12.f + 13.f, table_cell->preferred_width(&frndr));
}

TEST(table_cell_min_width, includes_margin) {
    fake_renderer  frndr;
    frndr.set_float_param("table_cell.margin_left", 12.0);
    frndr.set_float_param("table_cell.margin_right", 13.0);

    document doc = document::from_string("| lorem | ipsum |\n| --- | --- |\n| lorem | ipsum dolores |");
    // table -> table_row -> table_cell
    auto table = doc.get_root_element()->children()[0];
    auto table_row = table->children()[1]; // select the second row, not the header
    auto table_cell = table_row->children()[0];
    EXPECT_EQ(50.f + 12.f + 13.f, table_cell->min_width(&frndr));

    table_cell = table_row->children()[1];
    EXPECT_EQ(70.f + 12.f + 13.f, table_cell->min_width(&frndr));
}



#include "table_element.h"
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

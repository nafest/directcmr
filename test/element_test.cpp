#include "element.h"
#include "fake_renderer.h"
#include "gtest/gtest.h"

class text_element_test : public ::testing::Test {
  public:
    text_element_test() : ::testing::Test(), pstate(200) {}

  protected:
    virtual void SetUp() override {
        fnt = frndr.create_font("courier", "bold", 10);
        text.set_font(fnt);
        // the fake renderer estimates a constant width of
        // 10 for each character:
        // 30 + 10 + 40 + 10 + 30 + 10 + 50
        text.set_literal("The lazy fox jumps");
    }

    virtual void TearDown() override {}

    fake_renderer frndr;
    font *fnt;
    text_element text;
    paragraph_state pstate;
};

TEST_F(text_element_test, add_to_leaf_node_is_correct) {
    text.add_to_leaf_block(&frndr, pstate);
    EXPECT_EQ(190, pstate.get_posx());
    EXPECT_EQ(0, pstate.get_posy());
    EXPECT_EQ(15, pstate.get_line_height());
}

TEST_F(text_element_test, add_to_leaf_node_is_correct_with_wrap) {
    pstate.set_paragraph_width(100);
    text.add_to_leaf_block(&frndr, pstate);
    EXPECT_EQ(100, pstate.get_posx());
    EXPECT_EQ(15, pstate.get_posy());
    EXPECT_EQ(15, pstate.get_line_height());
}

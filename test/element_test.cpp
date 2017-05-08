#include "element.h"
#include "fake_backend.h"
#include "text_element.h"
#include "gtest/gtest.h"

class text_element_test : public ::testing::Test {
  public:
    text_element_test() : ::testing::Test() {}

  protected:
    virtual void SetUp() override {
        cmr::style st;
        st.set_strong(true);
        text.propagate_style(st);

        // the fake renderer estimates a constant width of
        // 10 for each character:
        // 30 + 10 + 40 + 10 + 30 + 10 + 50
        text.set_literal("The lazy fox jumps");
        pstate =
            cmr::paragraph_state(200, text.get_font(&fbcknd)->get_line_height(),
                                 text.get_font(&fbcknd)->get_ascent());
    }

    virtual void TearDown() override {}

    fake_backend fbcknd;
    cmr::font *fnt;
    cmr::text_element text;
    cmr::paragraph_state pstate;
};

TEST_F(text_element_test, add_to_leaf_node_is_correct) {
    text.add_to_leaf_block(&fbcknd, pstate);
    EXPECT_EQ(180, pstate.get_posx());
    EXPECT_EQ(0, pstate.get_posy());
    EXPECT_EQ(text.get_font(&fbcknd)->get_line_height(),
              pstate.get_line_height());
}

TEST_F(text_element_test, add_to_leaf_node_is_correct_with_wrap) {
    pstate.set_paragraph_width(100);
    text.add_to_leaf_block(&fbcknd, pstate);
    EXPECT_EQ(90, pstate.get_posx());
    EXPECT_EQ(text.get_font(&fbcknd)->get_line_height(), pstate.get_posy());
    EXPECT_EQ(text.get_font(&fbcknd)->get_line_height(),
              pstate.get_line_height());
}

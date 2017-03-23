#pragma once

#include "element.h"

class leaf_block_element : public element {
  public:
    virtual float layout(renderer *rndr, float width) override {
        // a paragraph is a leaf block, i.e. it may not
        // contain other blocks. Only text and inline
        // elements

        // basically we have to start at the top left of
        // the block and subsequently add all child elements
        // also keep track of the height of the current line
        paragraph_state pstate(width, get_font(rndr)->get_line_height(),
                               get_font(rndr)->get_ascent());
        for (auto child : m_children) {
            child->add_to_leaf_block(rndr, pstate);
        }

        // add the line height for the last line only, if at least
        // one word has been added to the last line
        return pstate.get_posy() + pstate.get_posy() > 0.f
                   ? pstate.get_line_height()
                   : 0.f - get_font(rndr)->get_ascent();
    }
};

#pragma once

#include "element.h"

class paragraph_element : public element {
  public:
    float layout(renderer *rndr, float width) override {
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

        return pstate.get_posy() + pstate.get_line_height() -
               get_font(rndr)->get_ascent();
    }
};

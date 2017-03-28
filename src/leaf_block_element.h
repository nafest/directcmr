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
        // to use the correct space widths, add the spaces around
        // child elements in this element, e.g. otherwise there would be
        // a too wide space after an inline code span
        auto space_extents = get_font(rndr)->get_space_width();
        for (auto child : m_children) {
            child->add_to_leaf_block(rndr, pstate);
            pstate.add_space(space_extents);
        }

        // add the line height for the last line only, if at least
        // one word has been added to the last line
        return pstate.get_posy() + pstate.get_posy() > 0.f
                   ? pstate.get_line_height()
                   : 0.f - get_font(rndr)->get_ascent();
    }
};

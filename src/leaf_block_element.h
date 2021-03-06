// directcmr - the CommonMark renderer you never wanted
//
// leaf_block_element.h - common base clase for all leaf
//                        block elements (e.g. paragraph
//                        elements)
//
// Copyright (c) 2017 Stefan Winkler
// License: MIT License (for full license see LICENSE)

#pragma once

#include "element.h"

namespace dcmr {

class leaf_block_element : public element {
  public:
    leaf_block_element() : m_alignment(vertical_alignment::left) {}

    virtual float preferred_width(backend *bcknd) override {
        auto space_extents = get_font(bcknd)->get_space_width();
        auto margin = bcknd->get_style_sheet().get_margin(get_type());
        float preferred_width = 0.f;
        for (auto child : m_children) {
            preferred_width += child->preferred_width(bcknd) + space_extents;
        }
        return preferred_width - space_extents + margin.horizontal_margin();
    }

    virtual float layout(backend *bcknd, float width) override {
        // a paragraph is a leaf block, i.e. it may not
        // contain other blocks. Only text and inline
        // elements

        // subtract the margins before layouting
        auto margin = bcknd->get_style_sheet().get_margin(get_type());
        width -= margin.horizontal_margin();

        // basically we have to start at the top left of
        // the block and subsequently add all child elements
        // also keep track of the height of the current line
        paragraph_state pstate(width, get_font(bcknd)->get_line_height(),
                               get_font(bcknd)->get_ascent(), margin.top,
                               margin.left, m_alignment);
        // to use the correct space widths, add the spaces around
        // child elements in this element, e.g. otherwise there would be
        // a too wide space after an inline code span
        auto space_extents = get_font(bcknd)->get_space_width();
        for (auto child : m_children) {
            child->add_to_leaf_block(bcknd, pstate);
            pstate.add_space(space_extents);
        }

        pstate.set_base_line();

        // add the line height for the last line only, if at least
        // one word has been added to the last line
        return pstate.get_posy() + pstate.get_line_height() + margin.bottom;
    }

    void set_vertical_alignment(vertical_alignment alignment) {
        m_alignment = alignment;
    }

  private:
    vertical_alignment m_alignment;
};
}
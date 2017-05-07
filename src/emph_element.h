// cmark_renderer - a renderer for CommonMark
//
// emph_element.h - class for emphasized spans
//
// Copyright (c) 2017 Stefan Winkler
// License: MIT License (for full license see LICENSE)

#pragma once

#include "element.h"

namespace cmr {

class emph_element : public element {
  public:
    virtual void propagate_style(style st) noexcept override {
        st.set_emph(true);
        element::propagate_style(st);
    }

    virtual void add_to_leaf_block(renderer *rndr,
                                   paragraph_state &pstate) override {
        for (auto child : m_children)
            child->add_to_leaf_block(rndr, pstate);
    }
};
}

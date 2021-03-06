// directcmr - the CommonMark renderer you never wanted
//
// emph_element.h - class for emphasized spans
//
// Copyright (c) 2017 Stefan Winkler
// License: MIT License (for full license see LICENSE)

#pragma once

#include "element.h"

namespace dcmr {

class emph_element : public element {
  public:
    virtual void propagate_style(style st) noexcept override {
        st.set_emph(true);
        element::propagate_style(st);
    }

    virtual void add_to_leaf_block(backend *bcknd,
                                   paragraph_state &pstate) override {
        for (auto child : m_children)
            child->add_to_leaf_block(bcknd, pstate);
    }
};
}

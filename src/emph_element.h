#pragma once

#include "element.h"

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

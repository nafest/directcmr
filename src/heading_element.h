#pragma once

#include "element.h"

class heading_element : public element {
  public:
    heading_element(int heading_level)
        : element(), m_heading_level(heading_level) {}

    virtual void propagate_style(style st) noexcept override {
        st.set_heading_level(m_heading_level);
        element::propagate_style(st);
    }

    float layout(renderer *rndr, float width) override {
        // handle this like a paragraph

        paragraph_state pstate(width, get_font(rndr)->get_line_height(),
                               get_font(rndr)->get_ascent());

        for (auto child : m_children) {
            child->add_to_leaf_block(rndr, pstate);
        }

        return pstate.get_posy() + pstate.get_line_height();
    }

  private:
    int m_heading_level;
};

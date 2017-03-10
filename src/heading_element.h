#pragma once

#include "element.h"

#include <iostream>

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

        auto fnt = get_font(rndr);

        paragraph_state pstate(width, fnt->get_line_height(),
                               fnt->get_ascent());

        for (auto child : m_children) {
            child->add_to_leaf_block(rndr, pstate);
        }

        int height =
            pstate.get_posy() + pstate.get_line_height() - fnt->get_ascent();
        return height;
    }

  private:
    int m_heading_level;
};

#pragma once

#include "leaf_block_element.h"
#include "text_element.h"

#include <iostream>

class code_block_element : public leaf_block_element {
  public:
    virtual void propagate_style(style st) noexcept override {
        st.set_code(true);
        element::propagate_style(st);
    }

    virtual float layout(renderer *rndr, float width) override {
        auto margin = rndr->get_float_param("code_block.margin");
        m_pos.x() += margin;

        // Since code blocks pay respect to line breaks
        // layouting must be implemented different to leaf_block_element
        // It should be sufficient to count the number of lines here,
        // and return the number multiplied with the line height of the
        // font

        auto line_height = get_font(rndr)->get_line_height();

        return line_height * num_lines(m_literal);
    }

    virtual void render(renderer *rndr, vec2 pos) override {
        auto fnt = get_font(rndr);
        auto current_pos = pos + m_pos;
        current_pos.y() += fnt->get_ascent();
        for (auto &line : line_splitter(m_literal)) {
            std::string ln = line;
            rndr->draw_string(ln, current_pos, fnt);
            current_pos.y() += fnt->get_line_height();
        }
    }
};

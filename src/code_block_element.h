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
        auto margin = rndr->get_margin("code_block");
        // Since code blocks pay respect to line breaks
        // layouting must be implemented different to leaf_block_element
        // It should be sufficient to count the number of lines here,
        // and return the number multiplied with the line height of the
        // font

        auto line_height = get_font(rndr)->get_line_height();

        m_rect.bottom_right().x() = width;
        m_rect.bottom_right().y() = line_height * num_lines(m_literal) +
                                    m_rect.top_left().y() +
                                    margin.horizontal_margin();
        return line_height * num_lines(m_literal) + margin.top + margin.bottom;
    }

    virtual void render(renderer *rndr, vec2 pos) override {
        // draw the background rectangle, if it differs from the overall
        // background
        auto background_color =
            rndr->get_string_param("code_block.background_color");
        if (background_color != rndr->get_string_param("background_color")) {
            auto radius = rndr->get_float_param("code_block.border_radius");
            rndr->draw_rounded_rect(m_rect, radius, color(background_color));
        }
        auto fnt = get_font(rndr);
        auto current_pos = pos + m_rect.top_left();
        current_pos.x() += rndr->get_margin("code_block").left;
        current_pos.y() +=
            fnt->get_ascent() + rndr->get_margin("code_block").top;
        for (auto &line : line_splitter(m_literal)) {
            std::string ln = line;
            rndr->draw_string(ln, current_pos, fnt, get_color(rndr));
            current_pos.y() += fnt->get_line_height();
        }
    }
};

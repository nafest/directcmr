// cmark_renderer - a renderer for CommonMark
//
// code_block_element.h - class for code blocks
//
// Copyright (c) 2017 Stefan Winkler
// License: MIT License (for full license see LICENSE)

#pragma once

#include "leaf_block_element.h"
#include "text_element.h"

#include <iostream>

namespace cmr {

class code_block_element : public leaf_block_element {
  public:
    virtual void propagate_style(style st) noexcept override {
        st.set_code(true);
        element::propagate_style(st);
    }

    virtual float layout(backend *bcknd, float width) override {
        auto ss = bcknd->get_style_sheet();
        auto margin = ss.get_margin("code_block");
        // Since code blocks pay respect to line breaks
        // layouting must be implemented different to leaf_block_element
        // It should be sufficient to count the number of lines here,
        // and return the number multiplied with the line height of the
        // font

        auto line_height = get_font(bcknd)->get_line_height();

        m_rect.bottom_right().x() = width;
        m_rect.bottom_right().y() = line_height * num_lines(m_literal) +
            m_rect.top_left().y() +
            margin.vertical_margin();
        return line_height * num_lines(m_literal) + margin.top + margin.bottom;
    }

    virtual void render(backend *bcknd, vec2 pos) override {
        auto ss = bcknd->get_style_sheet();
        // draw the background rectangle, if it differs from the overall
        // background
        auto background_color =
            ss.get_string_param("code_block.background_color");
        if (background_color != ss.get_string_param("background_color")) {
            auto radius = ss.get_float_param("code_block.border_radius");
            bcknd->draw_rounded_rect(m_rect, radius, color(background_color));
        }
        auto fnt = get_font(bcknd);
        auto current_pos = pos + m_rect.top_left();
        current_pos.x() += ss.get_margin("code_block").left;
        current_pos.y() += ss.get_margin("code_block").top;
        for (auto &line : line_splitter(m_literal)) {
            std::string ln = line;
            bcknd->draw_string(ln, current_pos + vec2(0.f, fnt->get_ascent()), fnt, get_color(bcknd));
            current_pos.y() += fnt->get_line_height();
        }
    }
};
}
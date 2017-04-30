// cmark_renderer - a renderer for CommonMark
//
// code_element.h - class for inline code spans
//
// Copyright (c) 2017 Stefan Winkler
// License: MIT License (for full license see LICENSE)

#pragma once

#include "emph_element.h"

class code_element : public emph_element {
  public:
    virtual void add_to_leaf_block(renderer *rndr,
                                   paragraph_state &pstate) override {
        // add extra margin in the left;
        // manipulate the paragraph_state to do this;
        auto margin = rndr->get_margin("code");
        auto pwidth = pstate.get_width();
        pstate.set_paragraph_width(pwidth - margin.right);
        auto left_offset = pstate.get_left_offset();
        pstate.set_left_offset(left_offset + margin.left);
        pstate.advance(vec2(margin.left, 0.f), nullptr);

        for (auto child : m_children)
            child->add_to_leaf_block(rndr, pstate);

        pstate.advance(vec2(margin.right, 0.f), nullptr);
        pstate.set_paragraph_width(pwidth);
        pstate.set_left_offset(left_offset);
    }

    virtual void render(renderer *rndr, vec2 pos) override {
        // draw the background rectangle, if it differs from the overall
        // background
        auto margin = rndr->get_margin("code");
        auto background_color = rndr->get_string_param("code.background_color");
        if (background_color != rndr->get_string_param("background_color")) {
            auto radius = rndr->get_float_param("code.border_radius");

            // a code span contains only one text_element. So loop over all
            // text elements and form the background rectangles for this text;
            auto telem = m_children[0];

            auto &words = telem->children();

            if (words.size() < 1)
                return;

            auto font = words[0]->get_font(rndr);
            vec2 rect_offset = pos;
            rect_offset.x() -= margin.left;
            rect current_rect;
            current_rect.top_left() = words[0]->get_position();
            current_rect.bottom_right() = current_rect.top_left();
            auto extents = rndr->string_extents(font, words[0]->get_literal());
            current_rect.bottom_right().x() += extents.x();
            current_rect.bottom_right().y() += font->get_line_height();

            for (int i = 1; i < words.size(); i++) {
                auto extents =
                    rndr->string_extents(font, words[i]->get_literal());
                // are we still on the same line?
                if (words[i]->get_position().y() ==
                    current_rect.top_left().y()) {
                    current_rect.bottom_right().x() =
                        words[i]->get_position().x() + extents.x();
                } else { // no? -> draw the rect and start a new one
                    current_rect.top_left() += rect_offset;
                    current_rect.bottom_right() += rect_offset;
                    current_rect.bottom_right().x() +=
                        margin.horizontal_margin();
                    rndr->draw_rounded_rect(current_rect, radius,
                                            color(background_color));
                    current_rect.top_left() = words[i]->get_position();
                    current_rect.bottom_right() = current_rect.top_left();
                    current_rect.bottom_right().x() += extents.x();
                    current_rect.bottom_right().y() += font->get_line_height();
                }
            }
            current_rect.top_left() += rect_offset;
            current_rect.bottom_right() += rect_offset;
            current_rect.bottom_right().x() += margin.horizontal_margin();
            rndr->draw_rounded_rect(current_rect, radius,
                                    color(background_color));
        }

        emph_element::render(rndr, pos);
    }

    virtual void propagate_style(style st) noexcept override {
        st.set_inline_code(true);
        element::propagate_style(st);
    }
};

// cmark_renderer - a renderer for CommonMark
//
// text_element.h - class for text nodes. To store layouting
//                  information for rendering, these nodes
//                  are split to word nodes.
//
// Copyright (c) 2017 Stefan Winkler
// License: MIT License (for full license see LICENSE)

#pragma once

#include "element.h"
#include <iostream>

namespace cmr {

class word_element : public element {
  public:
    word_element(float top_to_base_line)
        : m_top_to_base_line(top_to_base_line) {}

    virtual float get_top_to_base_line() noexcept override { return m_top_to_base_line; }

    virtual void render(backend *bcknd, vec2 pos) override {
        pos.y() += m_top_to_base_line;
        bcknd->draw_string(m_literal, pos + m_rect.top_left(), get_font(bcknd),
                          get_color(bcknd));
    }

  private:
    float m_top_to_base_line;
};

class text_element : public element {
  public:
    text_element() = default;
    text_element(const std::string &literal) : element() {
        m_literal = literal;
    }

    // return the width of the widest word
    virtual float min_width(backend *bcknd) override {
        word_splitter splitter(m_literal);

        float min_width = 0.f;

        for (auto word : splitter) {
            auto extents = bcknd->string_extents(get_font(bcknd), word);
            min_width = std::max<float>(min_width, extents.x());
        }

        return min_width;
    }

    virtual float preferred_width(backend *bcknd) override {
        auto extents = bcknd->string_extents(get_font(bcknd), m_literal);
        return extents.x();
    }

    virtual void add_to_leaf_block(backend *bcknd,
                                   paragraph_state &pstate) override {
        word_splitter splitter(m_literal);

        std::vector<element *> word_children;

        auto space_extents = get_font(bcknd)->get_space_width();
        auto font = get_font(bcknd);

        for (auto word : splitter) {
            // Store the individual words with their layouted
            // position (top/left), such that these computations
            // must not be redone while rendering
            element *elem = new word_element(font->get_ascent());

            vec2 top_left{pstate.get_posx(), pstate.get_posy()};
            auto extents = bcknd->string_extents(font, word);
            if (!pstate.advance(extents, elem))
                top_left = vec2(pstate.get_left_offset(), pstate.get_posy());
            pstate.add_space(space_extents);

            elem->set_literal(word);
            elem->set_position(top_left);
            // word elements need to be aware of the style
            // of their parent
            elem->propagate_style(m_style);
            word_children.push_back(elem);
        }
        // undo the last space
        pstate.add_space(-space_extents);

        append_children(word_children);
    }
};
}

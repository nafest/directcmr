#pragma once

#include "element.h"
#include <iostream>

class word_element : public element {
  public:
    virtual void render(renderer *rndr, vec2 pos) override {
        rndr->draw_string(m_literal, pos + m_pos, get_font(rndr),
                          get_color(rndr));
    }
};

class text_element : public element {
  public:
    text_element() = default;
    text_element(const std::string &literal) : element() {
        m_literal = literal;
    }
    virtual void add_to_leaf_block(renderer *rndr,
                                   paragraph_state &pstate) override {
        word_splitter splitter(m_literal);

        std::vector<element *> word_children;

        auto space_extents = get_font(rndr)->get_space_width();

        for (auto word : splitter) {
            vec2 top_left{pstate.get_posx(), pstate.get_posy()};
            auto extents = rndr->string_extents(get_font(rndr), word);
            pstate.advance(extents);
            pstate.add_space(space_extents);

            // Store the individual words with their layouted
            // position (top/left), such that these computations
            // must not be redone while rendering
            element *elem = new word_element();
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

#pragma once

#include "paragraph_state.h"
#include "renderer.h"
#include "style.h"
#include "utils.h"

#include <string>
#include <vector>
#include <iostream>

class element {
  public:
    element() = default;
    virtual ~element() {
        for (auto child : m_children)
            delete child;
    }

    void set_children(const std::vector<element *> &children) {
        m_children = children;
    }

    std::vector<element *> &children() noexcept { return m_children; }

    void set_type(const std::string &type) noexcept { m_type = type; }

    void set_literal(const std::string &literal) noexcept {
        m_literal = literal;
    }

    virtual void propagate_style(style st = style()) noexcept {
        // the default implementation just passes the element's style to
        // all its child elements
        m_style = st;
        for (auto child : m_children) {
            child->propagate_style(m_style);
        }
    }

    style get_style() const noexcept { return m_style; }

    font *get_font(renderer *rndr) { return rndr->font_for_style(m_style); }

    virtual int layout(renderer *rndr, int width) { return 0; }
    virtual void render(renderer *rndr, position pos = {0, 0}) {
        for (auto child : m_children)
            child->render(rndr, pos + m_pos);
    }
    virtual void add_to_leaf_block(renderer *rndr, paragraph_state &pstate) {}

    void set_position(const position &pos) noexcept { m_pos = pos; }

  protected:
    // an element owns its child elements
    std::vector<element *> m_children;

    // human readable element type
    std::string m_type;

    // string literal
    std::string m_literal;

    style m_style;

    // position relative to the origin of the parent
    position m_pos;
};

class document_element : public element {
  public:
    virtual int layout(renderer *rndr, int width) {
        int cumul_height = 0;
        for (auto child : m_children) {
            child->set_position({0, cumul_height});
            cumul_height += child->layout(rndr, width);
        }

        return cumul_height;
    }
};

class word_element : public element {
  public:
    virtual void render(renderer *rndr, position pos) override {
        rndr->draw_string(m_literal, pos + m_pos, get_font(rndr));
    }
};

class text_element : public element {
  public:
    virtual void add_to_leaf_block(renderer *rndr,
                                   paragraph_state &pstate) override {
        auto words = split_string(m_literal);
        std::cout << words.size() << std::endl;
        std::vector<element *> word_children;

        auto space_extents = rndr->string_extents(get_font(rndr), " ");

        for (auto word : words) {
            position top_left{pstate.get_posx(), pstate.get_posy()};
            auto extents = rndr->string_extents(get_font(rndr), word);
            pstate.advance(extents);
            pstate.add_space(space_extents.width);

            // Store the individual words with their layouted
            // position (top/left), such that these computations
            // must not be redone while rendering
            element *elem = new word_element();
            elem->set_literal(word);
            elem->set_position(top_left);
            word_children.push_back(elem);
        }
        set_children(word_children);
    }
};

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

class heading_element : public element {
  public:
    heading_element(int heading_level)
        : element(), m_heading_level(heading_level) {}

    virtual void propagate_style(style st) noexcept override {
        st.set_heading_level(m_heading_level);
        element::propagate_style(st);
    }

    int layout(renderer *rndr, int width) override {
        // handle this like a paragraph

        paragraph_state pstate(width);

        for (auto child : m_children) {
            child->add_to_leaf_block(rndr, pstate);
        }

        return pstate.get_posy() + pstate.get_line_height();
    }

  private:
    int m_heading_level;
};

class paragraph_element : public element {
  public:
    int layout(renderer *rndr, int width) override {
        // a paragraph is a leaf block, i.e. it may not
        // contain other blocks. Only text and inline
        // elements

        // basically we have to start at the top left of
        // the block and subsequently add all child elements
        // also keep track of the height of the current line
        paragraph_state pstate(width);

        for (auto child : m_children) {
            child->add_to_leaf_block(rndr, pstate);
        }

        return pstate.get_posy() + pstate.get_line_height();
    }
};

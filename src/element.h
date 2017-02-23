#pragma once

#include "paragraph_state.h"
#include "renderer.h"
#include "style.h"
#include "utils.h"

#include <string>
#include <vector>

class element {
  public:
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

    void set_font(font *font) noexcept { m_font = font; }

    virtual int layout(renderer *rndr, int width) { return 0; }
    virtual void add_to_leaf_block(renderer *rndr, paragraph_state &pstate) {}

  protected:
    // an element owns its child elements
    std::vector<element *> m_children;

    // human readable element type
    std::string m_type;

    // string literal
    std::string m_literal;

    // font assigned to this element
    font *m_font;

    style m_style;
};

class text_element : public element {
  public:
    virtual void add_to_leaf_block(renderer *rndr,
                                   paragraph_state &pstate) override {
        auto words = split_string(m_literal);

        auto space_extents = rndr->string_extents(m_font, " ");

        for (auto word : words) {
            auto extents = rndr->string_extents(m_font, word);
            pstate.advance(extents);
            pstate.add_space(space_extents.width);

            // TODO: store the individual words with their layouted
            // position (top/left), such that these computations
            // must not be redone while rendering
        }
    }
};

class emph_element : public element {
  public:
    virtual void propagate_style(style st) noexcept override {
        st.set_emph(true);
        element::propagate_style(st);
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

#pragma once

#include "paragraph_state.h"
#include "renderer.h"
#include "style.h"
#include "utils.h"

#include <iostream>
#include <string>
#include <vector>

class element {
  public:
    element() = default;
    virtual ~element() {
        for (auto child : m_children)
            delete child;
    }

    void append_children(const std::vector<element *> &children) {
        m_children.insert(m_children.end(), children.begin(), children.end());
    }

    std::vector<element *> &children() noexcept { return m_children; }

    void set_type(const std::string &type) noexcept { m_type = type; }
    std::string get_type() const noexcept { return m_type; }

    void set_literal(const std::string &literal) noexcept {
        m_literal = literal;
    }

    std::string get_literal() const noexcept { return m_literal; }

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

    // layout the element and all its subelements to fit the given width.
    // Returns the height required to render the element
    virtual float layout(renderer *rndr, float width) {
        // on default return the cumulated height of all child elements
        float cumul_height = 0;
        for (auto child : m_children) {
            child->set_position({0, cumul_height});
            cumul_height += child->layout(rndr, width);
        }

        return cumul_height;
    }

    virtual void render(renderer *rndr, vec2 pos = {0, 0}) {
        for (auto child : m_children)
            child->render(rndr, pos + m_pos);
    }
    virtual void add_to_leaf_block(renderer *rndr, paragraph_state &pstate) {}

    void set_position(const vec2 &pos) noexcept { m_pos = pos; }
    vec2 get_position() const noexcept { return m_pos; }

  protected:
    // an element owns its child elements
    std::vector<element *> m_children;

    // human readable element type
    std::string m_type;

    // string literal
    std::string m_literal;

    style m_style;

    // position relative to the origin of the parent
    vec2 m_pos;
};

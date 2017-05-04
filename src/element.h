// cmark_renderer - a renderer for CommonMark
//
// element.h - base class for elements in the CommonMark
//             document tree
//
// Copyright (c) 2017 Stefan Winkler
// License: MIT License (for full license see LICENSE)

#pragma once

#include "paragraph_state.h"
#include "renderer.h"
#include "style.h"
#include "utils.h"

#include <algorithm>
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

    color get_color(renderer *rndr) {
        // the color of inline code spans dominates to support code spans
        // in blockquote elements
        if (get_style().get_inline_code()) {
            return string_to_color(rndr->get_string_param("code.color"));
        } else if (get_style().get_blockquote())
            return string_to_color(rndr->get_string_param("blockquote.color"));
        else if (get_style().get_code()) {
            return string_to_color(rndr->get_string_param("code_block.color"));
        } else if (get_style().get_inline_code()) {
            return string_to_color(rndr->get_string_param("code.color"));
        } else if (get_style().get_link()) {
            if (get_style().get_visited())
                return string_to_color(
                    rndr->get_string_param("link.visited_color"));
            else
                return string_to_color(rndr->get_string_param("link.color"));
        }

        return string_to_color(rndr->get_string_param("color"));
    }

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

    // return the minimum width required to render an element
    // (in most cases this should be the width of the largest word)
    virtual float min_width(renderer *rndr) {
        float min_width = 0;
        for (auto child : m_children) {
            min_width = std::max<float>(min_width, child->min_width(rndr));
        }
        return min_width + rndr->get_margin(get_type()).horizontal_margin();
    }

    // return the preferred width to render an element
    // (e.g. the width needed to render everything in a paragraph
    //  into one line)
    virtual float preferred_width(renderer *rndr) {
        float preferred_width = 0;
        for (auto child : m_children) {
            preferred_width =
                std::max<float>(preferred_width, child->preferred_width(rndr));
        }

        return preferred_width + rndr->get_margin(get_type()).vertical_margin();
    }

    virtual void render(renderer *rndr, vec2 pos = {0, 0}) {
        for (auto child : m_children)
            child->render(rndr, pos + m_rect.top_left());
    }

    virtual float get_top_to_base_line() noexcept { return 0.f; }

    virtual void add_to_leaf_block(renderer *rndr, paragraph_state &pstate) {}

    void set_position(const vec2 &pos) noexcept { m_rect.top_left() = pos; }
    vec2 get_position() const noexcept { return m_rect.top_left(); }

  protected:
    // an element owns its child elements
    std::vector<element *> m_children;

    // human readable element type
    std::string m_type;

    // string literal
    std::string m_literal;

    style m_style;

    // rectangle of the layouted element, margin is already excluded. In terms
    // of the parent
    // coordinate system. The bottom right may not be used by all elements.
    rect m_rect;
};

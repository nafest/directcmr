// cmark_renderer - a renderer for CommonMark
//
// blockqoute_element.h - class for blockquote blocks
//
// Copyright (c) 2017 Stefan Winkler
// License: MIT License (for full license see LICENSE)

#pragma once

#include "element.h"

namespace cmr {

class blockquote_element : public element {
  public:
    virtual float layout(backend *bcknd, float width) override {
        auto margin = bcknd->get_margin("blockquote");
        width -= margin.left;

        m_height = element::layout(bcknd, width);

        m_height += margin.top + margin.bottom;

        return m_height;
    }

    virtual void render(backend *bcknd, vec2 pos = {0, 0}) override {
        vec2 margin(bcknd->get_margin("blockquote").left,
                    bcknd->get_margin("blockquote").top);

        rect bq_marker(pos + m_rect.top_left() + vec2(0, 0),
                       pos + m_rect.top_left() + vec2(5, m_height));

        bcknd->draw_rect(bq_marker);
        for (auto child : m_children)
            child->render(bcknd, pos + m_rect.top_left() + margin);
    }
    virtual void propagate_style(style st) noexcept override {
        st.set_blockquote(true);
        element::propagate_style(st);
    }

  public:
    float m_height;
};
}

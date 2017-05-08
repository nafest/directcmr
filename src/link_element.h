/// cmark_renderer - a renderer for CommonMark
//
// link_element.h - class for link elements
//
// Copyright (c) 2017 Stefan Winkler
// License: MIT License (for full license see LICENSE)

#pragma once

#include "element.h"

namespace cmr {

class link_element : public element {
  public:
    link_element() = default;
    link_element(const std::string &url) : m_url(url) {}

    virtual void propagate_style(style st) noexcept override {
        st.set_link(true);
        element::propagate_style(st);
    }

    virtual void add_to_leaf_block(backend *bcknd,
                                   paragraph_state &pstate) override {
        // since propagate_style misses the bcknd as argument,
        // propagate the style for visited links right before
        // adding them to the current leaf block
        if (bcknd->is_visited_uri(m_url)) {
            m_style.set_visited(true);
            element::propagate_style(m_style);
        }
        for (auto child : m_children)
            child->add_to_leaf_block(bcknd, pstate);
    }
  private:
    std::string m_url;
};
}
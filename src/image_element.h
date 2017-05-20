// directcmr - the CommonMark renderer you never wanted
//
// image_element.h - class for image elements
//
// Copyright (c) 2017 Stefan Winkler
// License: MIT License (for full license see LICENSE)

#pragma once

#include "element.h"

namespace dcmr {

class image_element : public element {
  public:
    image_element(const std::string url) : m_src(url) {}

    virtual float get_top_to_base_line() noexcept override {
        return static_cast<float>(m_height);
    }

    virtual void add_to_leaf_block(backend *bcknd,
                                   paragraph_state &pstate) override {
        auto image_extents = bcknd->get_image_extents(m_src);
        auto margin = bcknd->get_style_sheet().get_margin("image");
        vec2 top_left{pstate.get_posx(), pstate.get_posy()};
        if (!pstate.advance(
                image_extents + vec2(margin.horizontal_margin(), 0.f), this)) {
            top_left = vec2(pstate.get_left_offset(), pstate.get_posy());
            // the image was added to the next line
            // scale it if necessary
            if (image_extents.x() + margin.horizontal_margin() <=
                pstate.get_width()) {
                m_width = static_cast<int>(image_extents.x());
                m_height = static_cast<int>(image_extents.y());
            } else {
                // keep the aspect ratio while using the maximum available width
                m_width = static_cast<int>(pstate.get_width() -
                                           margin.horizontal_margin());
                m_height = static_cast<int>(image_extents.y() *
                                            (m_width / image_extents.x()));
            }
        } else {
            m_width = static_cast<int>(image_extents.x());
            m_height = static_cast<int>(image_extents.y());
        }
        set_position(top_left);
    }

    virtual void render(backend *bcknd, vec2 pos) override {
        auto margin = bcknd->get_style_sheet().get_margin("image");
        bcknd->draw_image(m_src, pos + m_rect.top_left() +
                                     vec2(margin.left, margin.top),
                          m_width, m_height);
    }

  private:
    std::string m_src;
    int m_width;
    int m_height;
};
}
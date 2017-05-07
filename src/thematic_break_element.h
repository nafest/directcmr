// cmark_renderer - a renderer for CommonMark
//
// thematic_break_element.h - class for thematic breaks
//
// Copyright (c) 2017 Stefan Winkler
// License: MIT License (for full license see LICENSE)

#pragma once
#include "element.h"

namespace cmr {

class thematic_break_element : public element {
  public:
    float layout(renderer *rndr, float width) override {
        auto margin = rndr->get_margin("thematic_break");
        auto line_height = rndr->get_float_param("thematic_break.line_width");

        m_width = width - margin.horizontal_margin();

        return margin.vertical_margin() + line_height;
    }
    void render(renderer *rndr, vec2 pos) override {
        auto margin = rndr->get_margin("thematic_break");
        auto line_height = rndr->get_float_param("thematic_break.line_width");

        vec2 from = pos + m_rect.top_left() +
                    vec2(margin.left, margin.top + line_height * 0.5f);
        vec2 to = from + vec2(m_width, 0.0);

        color col(rndr->get_string_param("thematic_break.color"));

        rndr->draw_line(from, to, col, line_height);

        return;
    }

  private:
    float m_width;
};
}

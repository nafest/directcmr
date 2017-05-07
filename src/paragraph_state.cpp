// cmark_renderer - a renderer for CommonMark
//
// paragraph_state.cpp - current position for subsequently
//                       layouting elements in a paragraph
//
// Copyright (c) 2017 Stefan Winkler
// License: MIT License (for full license see LICENSE)

#include "paragraph_state.h"
#include "element.h"

void cmr::paragraph_state::set_base_line() {
    float max_dist_to_base_line = 0.f;

    for (auto elem : m_line_elements) {
        max_dist_to_base_line = std::max<float>(max_dist_to_base_line, elem->get_top_to_base_line());
    }

    float max_correction = 0.f;
    for (auto elem : m_line_elements) {
        auto correction = max_dist_to_base_line - elem->get_top_to_base_line();
        max_correction = std::max<float>(max_correction, correction);
        auto pos = elem->get_position();
        pos.y() += correction;
        elem->set_position(pos);
    }

    // on default the base line is offset with m_ascent. If
    // max_dist_to_base_line is larger than m_ascent, correct
    // for the difference. E.g. this is the case if there is
    // only a image and no text in a line.
    if (max_dist_to_base_line - m_ascent > max_correction)
        max_correction = max_dist_to_base_line - m_ascent;

    m_posy += max_correction;
}


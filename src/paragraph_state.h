// directcmr - the CommonMark renderer you never wanted
//
// paragraph_state.h - current position for subsequently
//                     layouting elements in a paragraph
//
// Copyright (c) 2017 Stefan Winkler
// License: MIT License (for full license see LICENSE)

#pragma once

#include "backend.h"

namespace dcmr {

class element;

class paragraph_state {
  public:
    paragraph_state() : m_posx(0), m_posy(0), m_line_height(0), m_width(0) {}

    paragraph_state(float paragraph_width, float line_height, float ascent,
                    float top_offset = 0.f, float left_offset = 0.f)
        : m_posx(left_offset), m_posy(top_offset), m_line_height(line_height),
          m_width(paragraph_width), m_left_offset(left_offset),
          m_ascent(ascent) {}

    bool advance(vec2 text_extents, element *elem) noexcept {
        if (text_extents.x() + m_posx - m_left_offset <= m_width ||
            // don't wrap the first word to the next line if it is wider than
            // the available paragraph width
            m_posx == m_left_offset) {
            // fits in the current line
            m_posx += text_extents.x();

            if (elem != nullptr)
                m_line_elements.push_back(elem);
            return true;
        } else {
            set_base_line();

            // advance to the next line
            m_posy += m_line_height;
            m_posx = text_extents.x() + m_left_offset;

            m_line_elements.clear();
            if (elem != nullptr)
                m_line_elements.push_back(elem);
            return false;
        }
    }

    void line_break() noexcept {
        set_base_line();
        m_posy += m_line_height;
        m_posx = m_left_offset;
        m_line_elements.clear();
    }

    void add_space(float space_width) noexcept {
        if (m_posx > m_left_offset)
            m_posx += space_width;
    }
    void set_base_line();

    float get_posx() const noexcept { return m_posx; }
    float get_posy() const noexcept { return m_posy; }
    float get_line_height() const noexcept { return m_line_height; }
    float get_left_offset() const noexcept { return m_left_offset; }
    float get_width() const noexcept { return m_width; }
    float get_ascent() const noexcept { return m_ascent; }

    void set_paragraph_width(float width) noexcept { m_width = width; }
    void set_left_offset(float left_offset) noexcept {
        m_left_offset = left_offset;
    }

  private:
    float m_posx;
    float m_posy;
    float m_line_height;
    float m_width;
    float m_left_offset;
    float m_ascent;

    std::vector<element *>
        m_line_elements; // keep track of all elements in a line, such
                         // that it is possible to adapt the vertical
                         // position of the base line
};
}
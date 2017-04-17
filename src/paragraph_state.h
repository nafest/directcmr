#pragma once

#include "renderer.h"

class paragraph_state {
  public:
    paragraph_state() : m_posx(0), m_posy(0), m_line_height(0), m_width(0) {}

    paragraph_state(float paragraph_width, float line_height, float top_offset = 0.f, float left_offset = 0.f)
        : m_posx(left_offset), m_posy(top_offset), m_line_height(line_height),
          m_width(paragraph_width), m_left_offset(left_offset) {}

    bool advance(vec2 text_extents) noexcept {
        if (text_extents.x() + m_posx - m_left_offset <= m_width ||
            // don't wrap the first word to the next line if it is wider than
            // the available paragraph width
            m_posx == m_left_offset) {
            // fits in the current line
            m_posx += text_extents.x();
            return true;
        } else {
            // advance to the next line
            m_posy += m_line_height;
            m_posx = text_extents.x() + m_left_offset;
            return false;
        }
    }

    void add_space(float space_width) noexcept { m_posx += space_width; }

    float get_posx() const noexcept { return m_posx; }
    float get_posy() const noexcept { return m_posy; }
    float get_line_height() const noexcept { return m_line_height; }
    float get_left_offset() const noexcept { return m_left_offset; }
    float get_width() const noexcept { return m_width; }

    void set_paragraph_width(float width) noexcept { m_width = width; }
    void set_left_offset(float left_offset) noexcept { m_left_offset = left_offset; }

  private:
    float m_posx;
    float m_posy;
    float m_line_height;
    float m_width;
    float m_left_offset;
};

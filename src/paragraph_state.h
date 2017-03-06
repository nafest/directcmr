#pragma once

#include "renderer.h"

class paragraph_state {
  public:
    paragraph_state(float paragraph_width, float top_offset = 0.f)
        : m_posx(0), m_posy(top_offset), m_line_height(0),
          m_width(paragraph_width) {}

    void advance(extents text_extents) noexcept {
        if (text_extents.width + m_posx < m_width) {
            // fits in the current line
            m_posx += text_extents.width;
            m_line_height = std::max(m_line_height, text_extents.height);
        } else {
            // advance to the next line
            m_posy += m_line_height;
            m_line_height = text_extents.height;
            m_posx = text_extents.width;
        }
    }

    void add_space(float space_width) noexcept { m_posx += space_width; }

    float get_posx() const noexcept { return m_posx; }
    float get_posy() const noexcept { return m_posy; }
    float get_line_height() const noexcept { return m_line_height; }

    void set_paragraph_width(float width) noexcept { m_width = width; }

  private:
    float m_posx;
    float m_posy;
    float m_line_height;
    float m_width;
};

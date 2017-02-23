#pragma once

#include "renderer.h"

class paragraph_state {
  public:
    paragraph_state(int paragraph_width)
        : m_posx(0), m_posy(0), m_line_height(0), m_width(paragraph_width) {}

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

    void add_space(int space_width) noexcept { m_posx += space_width; }

    int get_posx() const noexcept { return m_posx; }
    int get_posy() const noexcept { return m_posy; }
    int get_line_height() const noexcept { return m_line_height; }

    void set_paragraph_width(int width) noexcept { m_width = width; }

  private:
    int m_posx;
    int m_posy;
    int m_line_height;
    int m_width;
};



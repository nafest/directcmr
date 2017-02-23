#pragma once

class style {
  public:
    style() : m_heading_level(0), m_emph(false), m_strong(false) {}
    ~style() = default;

    void set_emph(bool emph) { m_emph = emph; }
    void set_strong(bool strong) { m_strong = strong; }
    void set_heading_level(bool heading_level) {
        m_heading_level = heading_level;
    }

  private:
    int m_heading_level;
    bool m_emph;
    bool m_strong;
};

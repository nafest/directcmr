#pragma once

class style {
  public:
    style() : m_heading_level(0), m_emph(false), m_strong(false) {}
    ~style() = default;

    void set_emph(bool emph) noexcept { m_emph = emph; }
    bool get_emph() const noexcept { return m_emph; }

    void set_strong(bool strong) noexcept { m_strong = strong; }
    bool get_strong() const noexcept { return m_strong; }

    void set_heading_level(int heading_level) noexcept {
        m_heading_level = heading_level;
    }
    int get_heading_level() const noexcept { return m_heading_level; }

  private:
    int m_heading_level;
    bool m_emph;
    bool m_strong;
};

// cmark_renderer - a renderer for CommonMark
//
// geom.h - 2d geometry utils
//
// Copyright (c) 2017 Stefan Winkler
// License: MIT License (for full license see LICENSE)

#pragma once

class vec2 {
  public:
    vec2() : m_x(0), m_y(0) {}
    vec2(float x, float y) : m_x(x), m_y(y) {}

    vec2 operator+(const vec2 &rhs) {
        return vec2(m_x + rhs.x(), m_y + rhs.y());
    }

    bool operator==(const vec2 &rhs) const {
        return m_x == rhs.x() && m_y == rhs.y();
    }

    float &x() noexcept { return m_x; }
    const float &x() const noexcept { return m_x; }

    float &y() noexcept { return m_y; }
    const float &y() const noexcept { return m_y; }

  private:
    float m_x;
    float m_y;
};

class rect {
  public:
    rect() = default;
    rect(const vec2 &top_left, const vec2 &bottom_right)
        : m_top_left(top_left), m_bottom_right(bottom_right) {}
    rect(float top, float left, float bottom, float right)
        : m_top_left({top, left}), m_bottom_right({bottom, right}) {}

    float width() const noexcept { return m_bottom_right.x() - m_top_left.x(); }
    float height() const noexcept {
        return m_bottom_right.y() - m_top_left.y();
    }

    vec2 &top_left() noexcept { return m_top_left; }
    const vec2 &top_left() const noexcept { return m_top_left; }

    vec2 &bottom_right() noexcept { return m_bottom_right; }
    const vec2 &bottom_right() const noexcept { return m_bottom_right; }

  private:
    vec2 m_top_left;
    vec2 m_bottom_right;
};

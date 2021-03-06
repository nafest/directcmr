// directcmr - the CommonMark renderer you never wanted
//
// style.h - class for styling information that is
//           propagated through the element tree
//
// Copyright (c) 2017 Stefan Winkler
// License: MIT License (for full license see LICENSE)

#pragma once

namespace dcmr {

enum class vertical_alignment { left, center, right };

class style {
  public:
    style()
        : m_heading_level(0), m_emph(false), m_strong(false), m_code(false),
          m_inline_code(false), m_blockquote(false), m_link(false),
          m_visited(false) {}
    ~style() = default;

    void set_emph(bool emph) noexcept { m_emph = emph; }
    bool get_emph() const noexcept { return m_emph; }

    void set_strong(bool strong) noexcept { m_strong = strong; }
    bool get_strong() const noexcept { return m_strong; }

    void set_code(bool is_code_block) noexcept { m_code = is_code_block; }
    bool get_code() const noexcept { return m_code; }

    void set_inline_code(bool is_inline_code) noexcept {
        m_inline_code = is_inline_code;
    }
    bool get_inline_code() const noexcept { return m_inline_code; }

    void set_blockquote(bool is_blockquote) noexcept {
        m_blockquote = is_blockquote;
    }
    bool get_blockquote() const noexcept { return m_blockquote; }

    void set_link(bool is_link) noexcept { m_link = is_link; }
    bool get_link() const noexcept { return m_link; }

    void set_visited(bool is_visited) noexcept { m_visited = is_visited; }
    bool get_visited() const noexcept { return m_visited; }

    void set_heading_level(int heading_level) noexcept {
        m_heading_level = heading_level;
    }
    int get_heading_level() const noexcept { return m_heading_level; }

  private:
    int m_heading_level;
    bool m_emph;
    bool m_strong;
    bool m_code;
    bool m_inline_code;
    bool m_blockquote;
    bool m_link;
    bool m_visited;
};

inline bool operator<(const style &lhs, const style &rhs) {
    if (lhs.get_heading_level() < rhs.get_heading_level())
        return true;
    if (lhs.get_heading_level() > rhs.get_heading_level())
        return false;

    if (!lhs.get_emph() && rhs.get_emph())
        return true;
    if (lhs.get_emph() && !rhs.get_emph())
        return false;

    if (!lhs.get_strong() && rhs.get_strong())
        return true;
    if (lhs.get_strong() && !rhs.get_strong())
        return false;

    if (!lhs.get_code() && rhs.get_code())
        return true;
    if (lhs.get_code() && !rhs.get_code())
        return false;

    if (!lhs.get_inline_code() && rhs.get_inline_code())
        return true;
    if (lhs.get_inline_code() && !rhs.get_inline_code())
        return false;

    if (!lhs.get_blockquote() && rhs.get_blockquote())
        return true;
    if (lhs.get_blockquote() && !rhs.get_blockquote())
        return false;

    if (!lhs.get_link() && rhs.get_link())
        return true;
    if (lhs.get_link() && !rhs.get_link())
        return false;

    if (!lhs.get_visited() && rhs.get_visited())
        return true;
    if (lhs.get_visited() && !rhs.get_visited())
        return false;

    return false;
}
}
#pragma once

#include <string>

struct extents {
    int width;
    int height;
};

// class representing a font. Implementations of renderer
// may subclass it to extend it with render backend specific
// data
class font {
  public:
    font() = default;
    virtual ~font() = default;

    void set_line_height(int line_height) noexcept {
        m_line_height = line_height;
    }

    int get_line_height() const noexcept { return m_line_height; }

  private:
    int m_line_height;
};

// abstract class for the interface of a renderer
class renderer {
  public:
    virtual extents string_extents(const font *fnt, const std::string &string) = 0;
    virtual font *create_font(const std::string &family,
                              const std::string &style, int size) = 0;
};

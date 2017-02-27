#pragma once

#include "style.h"

#include <map>
#include <string>

struct extents {
    int width;
    int height;
};

class position {
  public:
    position() : x(0), y(0) {}
    position(int x_, int y_) : x(x_), y(y_) {}

    int x;
    int y;

    position operator+(const position &rhs) {
        return position(x + rhs.x, y + rhs.y);
    }
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
    virtual extents string_extents(const font *fnt,
                                   const std::string &string) = 0;
    virtual font *create_font(const std::string &family,
                              const std::string &style, int size) = 0;
    virtual std::string default_family() const noexcept { return "Arial"; }
    virtual int default_size() const noexcept { return 10; }
    font *font_for_style(const style &st);

    virtual void draw_string(const std::string& text, const position& pos, font *fnt) = 0;

  private:
    std::map<style, font *> m_cached_fonts;
};

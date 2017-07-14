// directcmr - the CommonMark renderer you never wanted
//
// backend.h - abstract base class for renderer backends
//
// Copyright (c) 2017 Stefan Winkler
// License: MIT License (for full license see LICENSE)

#pragma once

#include "geom.h"
#include "style.h"
#include "style_sheet.h"
#include "utils.h"

#include <map>
#include <string>

namespace dcmr {

// class representing a font. Implementations of backend
// must subclass it to extend it with render backend specific
// data
class font {
  public:
    font() = default;
    virtual ~font() = default;

    // Returns the line height. In most case this should
    // be ascent + descent + leading.
    virtual float get_line_height() const noexcept = 0;

    // Return the font's ascent.
    virtual float get_ascent() const noexcept = 0;

    // Return the width of an uppercase X.
    virtual float get_x_width() const noexcept = 0;

    // Get the width required for a whitespace character.
    virtual float get_space_width() const noexcept = 0;

    // Return the family of the font.
    virtual std::string get_family() const noexcept = 0;
};

// struct for rgba colors
struct color {
    color(unsigned char _r, unsigned char _g, unsigned char _b,
          unsigned char _a)
        : r(_r), g(_g), b(_b), a(_a) {}
    color(const std::string &color_string) {
        *this = string_to_color(color_string);
    }
    bool operator==(const color &rhs) const {
        return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a;
    }
    unsigned char r, g, b, a;
};

inline std::ostream &operator<<(std::ostream &os, const color &c) {
    return os << "(" << c.r << ", " << c.g << ", " << c.b << ", " << c.a << ")";
}

// abstract class for the interface of a renderer backend
// subclass and implement all pure virtual methods.
class backend {
  public:
    // called immediately before rendering starts
    //
    // width - the value passed to document::layout
    // height - the height passed to document::render
    virtual void prepare_canvas(int width, int height) = 0;

    // return the the extents of a string
    //
    // fnt - the font, which will be used to render the string
    // string - string to render
    virtual vec2 string_extents(const font *fnt, const std::string &string) = 0;

    // create a new font
    //
    // family - required font family
    // style - "Normal", "Bold", "Italic", or "BoldItalic"
    virtual font *create_font(const std::string &family,
                              const std::string &style, int size) = 0;

    // returns a font that fits a given style. Implements a cache
    // for fonts
    font *font_for_style(const style &st);

    // return the extents of an image
    //
    // src - source string of the image
    virtual vec2 get_image_extents(const std::string &src) = 0;

    // draw an image
    //
    // src - source string of the image
    // pos - position of the top left corner of the image
    // width - width available to draw the image
    // height - height available to draw the image
    //
    // draw_image should scale the image to meet width and height
    virtual void draw_image(const std::string &src, const vec2 &pos, int width,
                            int height) = 0;

    // draw a string
    //
    // text - string to draw in UTF-8 encoding
    // pos - position of the base line to render the string
    // fnt - font
    // color - color of the string
    virtual void draw_string(const std::string &text, const vec2 &pos,
                             font *fnt, const color &col) = 0;

    // draw the marker of an unordered list
    //
    // marker_rect - rectangle enclosing the marker
    // col - color of the marker
    virtual void draw_list_marker(const rect &marker_rect,
                                  const color &col) = 0;

    // draw a rounded rectangle
    //
    // rectangle - bounds of the rectangle
    // radius - radius of the corners, may be zero
    // col - color of the rectangle
    // fill - if true, the rectangle is filled
    virtual void draw_rounded_rect(const rect &rectangle, float radius,
                                   const color &col, bool fill = true) = 0;

    // draw a line
    //
    // from - start point of the line
    // to - end point of the line
    // col - color of the line
    // line_width - width of the line
    virtual void draw_line(const vec2 &from, const vec2 &to, const color &col,
                           float line_width = 1.f) = 0;

    // return true if the given URI has been visited before
    //
    // directcmr uses this to choose between link.color
    // and link.visited_color
    virtual bool is_visited_uri(const std::string &uri) = 0;

    // set the current clipping rectangle
    //
    // rectangle - drawing should be restricted to this rectangle
    virtual void set_clip_rect(const rect &rectangle) = 0;

    // save the current state (clipping rect)
    virtual void save() = 0;

    // restore the last state (clipping rect);
    virtual void restore() = 0;

    // return a reference to the style sheet
    style_sheet &get_style_sheet() { return m_style_sheet; }

  private:
    std::map<style, font *> m_cached_fonts;

    style_sheet m_style_sheet;
};
}

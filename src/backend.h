// cmark_renderer - a renderer for CommonMark
//
// backend.h - abstract base class for renderer backends
//
// Copyright (c) 2017 Stefan Winkler
// License: MIT License (for full license see LICENSE)

#pragma once

#include "geom.h"
#include "style.h"
#include "utils.h"

#include <map>
#include <string>

namespace cmr {

// class representing a font. Implementations of renderer
// may subclass it to extend it with render backend specific
// data
class font {
  public:
    font() = default;
    virtual ~font() = default;

    virtual float get_line_height() const noexcept = 0;

    virtual float get_ascent() const noexcept = 0;

    virtual float get_x_width() const noexcept = 0;

    virtual float get_space_width() const noexcept = 0;

    virtual std::string get_family() const noexcept = 0;
};

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

struct elem_margin {
  elem_margin(float _left, float _top, float _right, float _bottom)
      : top(_top), left(_left), right(_right), bottom(_bottom) {}
  float vertical_margin() const { return top + bottom; }
  float horizontal_margin() const { return left + right; }
  float top, left, bottom, right;
};

// abstract class for the interface of a renderer backend
class backend {
  public:
    backend();

    virtual float get_float_param(const std::string &param_name) const
    noexcept {
        auto elem = m_float_params.find(param_name);
        if (elem == m_float_params.end()) {
            auto dot_pos = param_name.find(".");
            if (dot_pos != std::string::npos) {
                return get_float_param(param_name.substr(dot_pos + 1));
            }
            return 0.f;
        }
        return elem->second;
    }

    virtual void set_float_param(const std::string &param_name,
                                 float value) noexcept {
        m_float_params[param_name] = value;
    }

    virtual std::string get_string_param(const std::string &param_name) const
    noexcept {
        auto elem = m_string_params.find(param_name);
        if (elem == m_string_params.end()) {
            auto dot_pos = param_name.find(".");
            if (dot_pos != std::string::npos) {
                return get_string_param(param_name.substr(dot_pos + 1));
            }
            return "";
        }
        return elem->second;
    }

    virtual void set_string_param(const std::string &param_name,
                                  const std::string &value) noexcept {
        m_string_params[param_name] = value;
    }

    virtual elem_margin get_margin(const std::string &element_name) const
    noexcept;

    virtual void prepare_canvas(int width, int height) = 0;
    virtual vec2 string_extents(const font *fnt, const std::string &string) = 0;
    virtual font *create_font(const std::string &family,
                              const std::string &style, int size) = 0;
    virtual std::string default_family() const noexcept { return "Arial"; }
    virtual int default_size() const noexcept { return 14; }
    virtual int heading_size(int heading_level) {
        int size = default_size();
        for (int i = 6; i >= heading_level; i--)
            size = static_cast<int>(size * 1.1f);

        return size;
    }

    font *font_for_style(const style &st);

    virtual vec2 get_image_extents(const std::string &src) = 0;
    virtual void draw_image(const std::string &src, const vec2 &pos, int width,
                            int height) = 0;

    virtual void draw_string(const std::string &text, const vec2 &pos,
                             font *fnt, const color &col) = 0;

    virtual void draw_list_marker(const rect &marker_rect) = 0;
    virtual void draw_rect(const rect &marker_rect) = 0;
    virtual void draw_rounded_rect(const rect &rectangle, float radius,
                                   const color &col, bool fill = true) = 0;
    virtual void draw_line(const vec2 &from, const vec2 &to, const color &col,
                           float line_width = 1.f) = 0;
    // return true if the given URI has been visited before
    //
    // cmark_renderer uses this to choose between link.color
    // and link.visited_color
    virtual bool is_visited_uri(const std::string &uri) = 0;

  private:
    float get_side_margin(const std::string &element_name,
                          const std::string side) const noexcept;

    std::map<style, font *> m_cached_fonts;
    std::map<std::string, float> m_float_params;
    std::map<std::string, std::string> m_string_params;
};
}

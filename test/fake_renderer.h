#pragma once

#include "renderer.h"

#include <iostream>

class font_params {
  public:
    font_params() = default;
    font_params(const std::string &family, const std::string &style, int size)
        : m_family(family), m_style(style), m_size(size) {}

    std::string m_family;
    std::string m_style;
    int m_size;
};

class fake_font : public font {
    virtual float get_line_height() const noexcept override {
        return m_font_params.m_size + 2.f;
    }

    virtual float get_ascent() const noexcept override {
        return m_font_params.m_size - 2.f;
    }

    virtual float get_x_width() const noexcept override {
        return m_font_params.m_size / 2.f;
    }

    virtual float get_space_width() const noexcept override {
        float char_width = 10.f;
        if (m_font_params.m_family == "Menlo")
            char_width = 12.f;
        return char_width;
    }

    virtual std::string get_family() const noexcept override {
        return m_font_params.m_family;
    }

  public:
    font_params m_font_params;
};

class draw_string_action {
  public:
    draw_string_action() = default;
    draw_string_action(const std::string &text, const vec2 &pos,
                       const font_params &font, const color &col)
        : m_text(text), m_pos(pos), m_font(font), m_color(col) {}

    font_params m_font;
    std::string m_text;
    vec2 m_pos;
    color m_color;
};

class draw_marker_action {
  public:
    draw_marker_action() = default;
    draw_marker_action(const rect &marker_rect) : m_marker_rect(marker_rect) {}

    rect m_marker_rect;
};

class fake_renderer : public renderer {
  public:
    virtual void prepare_canvas(int width, int height) override {}
    virtual vec2 string_extents(const font *fnt,
                                const std::string &string) override {
        const fake_font *ffnt = static_cast<const fake_font *>(fnt);
        float char_width = 10.f;
        if (ffnt->m_font_params.m_family == "Menlo")
            char_width = 12.f;
        return vec2(string.length() * char_width,
                    static_cast<float>(ffnt->m_font_params.m_size));
    }

    virtual font *create_font(const std::string &family,
                              const std::string &style, int size) override {
        fake_font *fnt = new fake_font();
        fnt->m_font_params = font_params(family, style, size);
        return fnt;
    }

    virtual void draw_string(const std::string &text, const vec2 &pos,
                             font *fnt_in, const color &col) override {
        fake_font *fnt = static_cast<fake_font *>(fnt_in);
        m_draw_string_calls.push_back(
            draw_string_action(text, pos, fnt->m_font_params, col));
    }

    virtual void draw_list_marker(const rect &marker_rect) override {
        m_draw_marker_calls.push_back(draw_marker_action(marker_rect));
    }

    virtual void draw_rect(const rect &marker_rect) override {
        m_draw_marker_calls.push_back(draw_marker_action(marker_rect));
    }

    virtual void draw_rounded_rect(const rect &rectangle, float radius,
                                   const color &col, bool fill) override {
        m_draw_marker_calls.push_back(draw_marker_action(rectangle));
    }

    void draw_line(const vec2 &from, const vec2 &to, const color &col,
                   float line_width) override {}
    std::vector<draw_string_action> m_draw_string_calls;
    std::vector<draw_marker_action> m_draw_marker_calls;
};

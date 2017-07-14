#pragma once

#include "backend.h"

#include <iostream>
#include <map>
#include <utility>

class font_params {
  public:
    font_params() = default;
    font_params(const std::string &family, const std::string &style, int size)
        : m_family(family), m_style(style), m_size(size) {}

    std::string m_family;
    std::string m_style;
    int m_size;
};

class fake_font : public dcmr::font {
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
    draw_string_action(const std::string &text, const dcmr::vec2 &pos,
                       const font_params &font, const dcmr::color &col)
        : m_text(text), m_pos(pos), m_font(font), m_color(col) {}

    font_params m_font;
    std::string m_text;
    dcmr::vec2 m_pos;
    dcmr::color m_color;
};

class draw_marker_action {
  public:
    draw_marker_action() = default;
    draw_marker_action(const dcmr::rect &marker_rect)
        : m_marker_rect(marker_rect) {}

    dcmr::rect m_marker_rect;
};

class draw_rect_action {
  public:
    draw_rect_action() = default;
    draw_rect_action(const dcmr::rect &_rect) : m_rect(_rect) {}

    dcmr::rect m_rect;
};

class draw_image_action {
  public:
    draw_image_action() = default;
    draw_image_action(const std::string &src, const dcmr::vec2 &pos, int width,
                      int height)
        : m_src(src), m_pos(pos), m_width(width), m_height(height) {}

    std::string m_src;
    dcmr::vec2 m_pos;
    int m_width;
    int m_height;
};

class draw_line_action {
  public:
    draw_line_action() = default;
    draw_line_action(const dcmr::vec2 &from, const dcmr::vec2 &to,
                     const dcmr::color &col, float line_width)
        : m_from(from), m_to(to), m_color(col), m_line_width(line_width) {}

    dcmr::vec2 m_from, m_to;
    dcmr::color m_color;
    float m_line_width;
};

class fake_backend : public dcmr::backend {
  public:
    virtual void prepare_canvas(int width, int height) override {}
    virtual dcmr::vec2 string_extents(const dcmr::font *fnt,
                                      const std::string &string) override {
        const fake_font *ffnt = static_cast<const fake_font *>(fnt);
        float char_width = 10.f;
        if (ffnt->m_font_params.m_family == "Menlo")
            char_width = 12.f;
        return dcmr::vec2(string.length() * char_width,
                          static_cast<float>(ffnt->m_font_params.m_size));
    }

    virtual dcmr::font *create_font(const std::string &family,
                                    const std::string &style,
                                    int size) override {
        fake_font *fnt = new fake_font();
        fnt->m_font_params = font_params(family, style, size);
        return fnt;
    }

    virtual void draw_string(const std::string &text, const dcmr::vec2 &pos,
                             dcmr::font *fnt_in,
                             const dcmr::color &col) override {
        fake_font *fnt = static_cast<fake_font *>(fnt_in);
        m_draw_string_calls.push_back(
            draw_string_action(text, pos, fnt->m_font_params, col));
    }

    virtual void draw_list_marker(const dcmr::rect &marker_rect,
                                  const dcmr::color &col) override {
        m_draw_marker_calls.push_back(draw_marker_action(marker_rect));
    }

    virtual void draw_rounded_rect(const dcmr::rect &rectangle, float radius,
                                   const dcmr::color &col, bool fill) override {
        m_draw_rect_calls.push_back(draw_rect_action(rectangle));
    }

    void draw_line(const dcmr::vec2 &from, const dcmr::vec2 &to,
                   const dcmr::color &col, float line_width) override {
        m_draw_line_calls.push_back(
            draw_line_action(from, to, col, line_width));
    }

    virtual dcmr::vec2 get_image_extents(const std::string &src) override {
        auto extents = m_image_extents[src];
        return dcmr::vec2(static_cast<float>(extents.first),
                          static_cast<float>(extents.second));
    }

    virtual void draw_image(const std::string &src, const dcmr::vec2 &pos,
                            int width, int height) override {
        m_draw_image_calls.push_back(
            draw_image_action(src, pos, width, height));
    }

    void set_image_extents(const std::string &src, int width, int height) {
        m_image_extents[src] = std::make_pair(width, height);
    }

    void add_visited_link(const std::string &uri) {
        m_visited_links.push_back(uri);
    }

    virtual bool is_visited_uri(const std::string &uri) override {
        return std::find(m_visited_links.begin(), m_visited_links.end(), uri) !=
               m_visited_links.end();
    }

    void set_clip_rect(const dcmr::rect &rectangle) override {}
    void save() override {}
    void restore() override {}

    std::map<std::string, std::pair<int, int>> m_image_extents;
    std::vector<draw_image_action> m_draw_image_calls;
    std::vector<draw_string_action> m_draw_string_calls;
    std::vector<draw_marker_action> m_draw_marker_calls;
    std::vector<draw_rect_action> m_draw_rect_calls;
    std::vector<draw_line_action> m_draw_line_calls;
    std::vector<std::string> m_visited_links;
};

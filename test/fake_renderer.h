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
  public:
    font_params m_font_params;
};

class draw_string_action {
  public:
    draw_string_action() = default;
    draw_string_action(const std::string &text, const position &pos,
                       const font_params &font)
        : m_text(text), m_pos(pos), m_font(font) {}

    font_params m_font;
    std::string m_text;
    position m_pos;
};

class fake_renderer : public renderer {
  public:
    virtual extents string_extents(const font *fnt,
                                   const std::string &string) override {
        extents ext;
        ext.height = 15;
        ext.width = string.length() * 10;

        return ext;
    }

    virtual font *create_font(const std::string &family,
                              const std::string &style, int size) override {
        fake_font *fnt = new fake_font();
        fnt->m_font_params = font_params(family, style, size);
        fnt->set_line_height(size + 2);
        return fnt;
    }

    virtual void draw_string(const std::string &text, const position &pos,
                             font *fnt_in) override {
        fake_font *fnt = static_cast<fake_font *>(fnt_in);
        m_draw_string_calls.push_back(
            draw_string_action(text, pos, fnt->m_font_params));
    }

    std::vector<draw_string_action> m_draw_string_calls;
};

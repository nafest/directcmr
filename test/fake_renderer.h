#pragma once

#include "renderer.h"

class fake_font : public font {};

class fake_renderer : public renderer {
  public:
    virtual extents string_extents(const font *fnt, const std::string &string) override {
        extents ext;
        ext.height = 15;;
        ext.width = string.length()*10;

        return ext;
    }

    virtual font *create_font(const std::string &family,
                              const std::string &style, int size) override {
        fake_font *fnt  = new fake_font();
        fnt->set_line_height(size + 2);
        return fnt;
    }
};

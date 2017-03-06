#pragma once

#include "SkCanvas.h"
#include "SkFontMgr.h"
#include "SkSurface.h"

#include "renderer.h"

class skia_renderer : public renderer {
  public:
    virtual void prepare_canvas(int width, int height) override;

    virtual font *create_font(const std::string &family,
                              const std::string &style, int size) override;

    virtual extents string_extents(const font *fnt,
                                   const std::string &string) override;

    virtual void draw_string(const std::string &text, const position &pos,
                             font *fnt_in) override;

    void dump_canvas(const std::string &file_name);

  private:
    sk_sp<SkSurface> m_surface;
    SkCanvas *m_canvas;
};

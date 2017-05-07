#pragma once

#include "SkCanvas.h"
#include "SkFontMgr.h"
#include "SkSurface.h"

#include "renderer.h"

class skia_renderer : public cmr::renderer {
  public:
    virtual void prepare_canvas(int width, int height) override;

    virtual cmr::vec2 string_extents(const cmr::font *fnt,
                                     const std::string &string) override;

    virtual cmr::font *create_font(const std::string &family,
                                   const std::string &style, int size) override;

    virtual cmr::vec2 get_image_extents(const std::string &src) override;

    virtual void draw_image(const std::string &src, const cmr::vec2 &pos,
                            int width, int height) override;

    virtual void draw_string(const std::string &text, const cmr::vec2 &pos,
                             cmr::font *fnt_in, const cmr::color &col) override;

    virtual void draw_list_marker(const cmr::rect &marker_rect) override;

    virtual void draw_rect(const cmr::rect &marker_rect) override;

    virtual void draw_rounded_rect(const cmr::rect &rectangle, float radius,
                                   const cmr::color &col,
                                   bool fill = true) override;

    virtual void draw_line(const cmr::vec2 &from, const cmr::vec2 &to,
                           const cmr::color &col,
                           float line_width = 1.f) override;

    virtual bool is_visited_uri(const std::string &uri) override;

    void dump_canvas(const std::string &file_name);

  private:
    sk_sp<SkSurface> m_surface;
    SkCanvas *m_canvas;
};

#pragma once

#include "SkCanvas.h"
#include "SkFontMgr.h"
#include "SkSurface.h"

#include "backend.h"

class skia_backend : public dcmr::backend {
  public:
    virtual void prepare_canvas(int width, int height) override;

    virtual dcmr::vec2 string_extents(const dcmr::font *fnt,
                                     const std::string &string) override;

    virtual dcmr::font *create_font(const std::string &family,
                                   const std::string &style, int size) override;

    virtual dcmr::vec2 get_image_extents(const std::string &src) override;

    virtual void draw_image(const std::string &src, const dcmr::vec2 &pos,
                            int width, int height) override;

    virtual void draw_string(const std::string &text, const dcmr::vec2 &pos,
                             dcmr::font *fnt_in, const dcmr::color &col) override;

    virtual void draw_list_marker(const dcmr::rect &marker_rect,
                                  const dcmr::color &col) override;

    virtual void draw_rounded_rect(const dcmr::rect &rectangle, float radius,
                                   const dcmr::color &col,
                                   bool fill = true) override;

    virtual void draw_line(const dcmr::vec2 &from, const dcmr::vec2 &to,
                           const dcmr::color &col,
                           float line_width = 1.f) override;

    virtual bool is_visited_uri(const std::string &uri) override;

    void dump_canvas(const std::string &file_name);

  private:
    sk_sp<SkSurface> m_surface;
    SkCanvas *m_canvas;
};

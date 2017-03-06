#include "skia_renderer.h"
#include "SkImage.h"
#include "SkStream.h"
#include "SkTypeface.h"

class skia_font : public font {
  public:
    skia_font(const std::string &family, const std::string &style, int size) {
        sk_sp<SkFontMgr> fmgr = SkFontMgr::RefDefault();

        int weight = SkFontStyle::kNormal_Weight;
        if (style == "Bold" || style == "BoldItalic")
            weight = SkFontStyle::kBold_Weight;

        SkFontStyle font_style{weight, SkFontStyle::kNormal_Width,
                               SkFontStyle::kUpright_Slant};

        m_typeface.reset(fmgr->matchFamilyStyle(family.c_str(), font_style));

        m_paint.setTypeface(m_typeface);
        m_paint.setTextSize(size);
        m_paint.setAntiAlias(true);
        m_paint.setColor(SK_ColorBLACK);
    }

    extents string_extents(const std::string &text) const {
        SkRect bounds;
        m_paint.measureText(text.c_str(), text.length(), &bounds);
        return extents(bounds.width(), bounds.height());
    }

    SkPaint &paint() { return m_paint; }

    virtual float get_line_height() const noexcept override { return 0.f; }

    virtual float get_ascent() const noexcept override {
        SkPaint::FontMetrics metrics;
        m_paint.getFontMetrics(&metrics);
        return -metrics.fAscent;
    }

  private:
    sk_sp<SkTypeface> m_typeface;
    SkPaint m_paint;
};

void skia_renderer::prepare_canvas(int width, int height) {
    m_surface = SkSurface::MakeRasterN32Premul(width, height);
    m_canvas = m_surface->getCanvas();
}

void skia_renderer::dump_canvas(const std::string &file_name) {
    sk_sp<SkImage> img = m_surface->makeImageSnapshot();
    sk_sp<SkData> data{img->encode(SkEncodedImageFormat::kPNG, 100)};
    SkFILEWStream out(file_name.c_str());
    out.write(data->bytes(), data->size());
}

font *skia_renderer::create_font(const std::string &family,
                                 const std::string &style, int size) {
    return new skia_font(family, style, size);
}

extents skia_renderer::string_extents(const font *fnt,
                                      const std::string &string) {
    extents ext;
    const skia_font *sfnt = static_cast<const skia_font *>(fnt);
    return sfnt->string_extents(string);
}

void skia_renderer::draw_string(const std::string &text, const position &pos,
                                font *fnt_in) {
    skia_font *fnt = static_cast<skia_font *>(fnt_in);
    m_canvas->drawText(text.c_str(), text.size(), pos.x, pos.y, fnt->paint());
}

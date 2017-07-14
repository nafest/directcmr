#include "skia_backend.h"
#include "../cmake-build-debug/thirdparty/libskia-prefix/src/libskia/include/core/SkRegion.h"
#include "../cmake-build-release/thirdparty/libskia-prefix/src/libskia/include/core/SkPaint.h"
#include "SkImage.h"
#include "SkImageDeserializer.h"
#include "SkRegion.h"
#include "SkStream.h"
#include "SkTypeface.h"

#include <algorithm>

class skia_font : public dcmr::font {
  public:
    skia_font(const std::string &family, const std::string &style, int size)
        : m_family(family) {
        sk_sp<SkFontMgr> fmgr = SkFontMgr::RefDefault();

        int weight = SkFontStyle::kNormal_Weight;
        if (style == "Bold" || style == "BoldItalic")
            weight = SkFontStyle::kBold_Weight;

        SkFontStyle::Slant slant = SkFontStyle::kUpright_Slant;
        if (style == "Italic" || style == "BoldItalic")
            slant = SkFontStyle::kItalic_Slant;

        SkFontStyle font_style{weight, SkFontStyle::kNormal_Width, slant};

        m_typeface.reset(fmgr->matchFamilyStyle(family.c_str(), font_style));

        m_paint.setTypeface(m_typeface);
        m_paint.setTextSize(size);
        m_paint.setAntiAlias(true);
        m_paint.setColor(SK_ColorBLACK);
        m_paint.getFontMetrics(&m_metrics);

        m_xwidth = string_extents("X").x();
        m_space_width = string_extents(" ").x();
    }

    dcmr::vec2 string_extents(const std::string &text) const {
        SkRect bounds;

        auto vertical_size =
            m_paint.measureText(text.c_str(), text.length(), &bounds);
        return dcmr::vec2(vertical_size, bounds.height());
    }

    SkPaint &paint() { return m_paint; }

    virtual float get_line_height() const noexcept override {
        return -m_metrics.fAscent + m_metrics.fDescent + m_metrics.fLeading;
    }

    virtual float get_ascent() const noexcept override {
        return -m_metrics.fAscent;
    }

    virtual float get_x_width() const noexcept override { return m_xwidth; }

    virtual float get_space_width() const noexcept override {
        return m_space_width;
    }

    virtual std::string get_family() const noexcept override {
        return m_family;
    }

  private:
    sk_sp<SkTypeface> m_typeface;
    SkPaint m_paint;
    SkPaint::FontMetrics m_metrics;
    float m_xwidth;
    float m_space_width;
    std::string m_family;
};

void skia_backend::prepare_canvas(int width, int height) {
    m_surface = SkSurface::MakeRasterN32Premul(width, height);
    m_canvas = m_surface->getCanvas();
    m_canvas->drawColor(SkColorSetARGBMacro(255, 255, 255, 255));
}

void skia_backend::dump_canvas(const std::string &file_name) {
    sk_sp<SkImage> img = m_surface->makeImageSnapshot();
    sk_sp<SkData> data{img->encode(SkEncodedImageFormat::kPNG, 100)};
    SkFILEWStream out(file_name.c_str());
    out.write(data->bytes(), data->size());
}

dcmr::font *skia_backend::create_font(const std::string &family,
                                      const std::string &style, int size) {
    return new skia_font(family, style, size);
}

dcmr::vec2 skia_backend::string_extents(const dcmr::font *fnt,
                                        const std::string &string) {
    const skia_font *sfnt = static_cast<const skia_font *>(fnt);
    return sfnt->string_extents(string);
}

void skia_backend::draw_string(const std::string &text, const dcmr::vec2 &pos,
                               dcmr::font *fnt_in, const dcmr::color &col) {
    skia_font *fnt = static_cast<skia_font *>(fnt_in);
    fnt->paint().setColor(SkColorSetARGBMacro(col.a, col.r, col.g, col.b));
    m_canvas->drawText(text.c_str(), text.size(), pos.x(), pos.y(),
                       fnt->paint());
}

void skia_backend::draw_list_marker(const dcmr::rect &marker_rect,
                                    const dcmr::color &col) {
    float cx = marker_rect.top_left().x() + 0.5 * marker_rect.width();
    float cy = marker_rect.top_left().y() + 0.5 * marker_rect.height();
    float radius = 0.2 * std::min(marker_rect.width(), marker_rect.height());
    SkPaint paint;
    paint.setColor(SkColorSetARGBMacro(col.a, col.r, col.g, col.b));
    paint.setAntiAlias(true);
    paint.setStyle(SkPaint::kStrokeAndFill_Style);
    m_canvas->drawCircle(cx, cy, radius, paint);
}

void skia_backend::draw_rounded_rect(const dcmr::rect &rectangle, float radius,
                                     const dcmr::color &col, bool fill) {
    SkPaint paint;
    paint.setColor(SkColorSetARGBMacro(col.a, col.r, col.g, col.b));
    paint.setAntiAlias(true);
    if (fill)
        paint.setStyle(SkPaint::kStrokeAndFill_Style);
    else
        paint.setStyle(SkPaint::kStroke_Style);
    paint.setStyle(SkPaint::kStrokeAndFill_Style);
    SkRect rect = SkRect::MakeLTRB(
        rectangle.top_left().x(), rectangle.top_left().y(),
        rectangle.bottom_right().x(), rectangle.bottom_right().y());
    if (radius == 0.f)
        m_canvas->drawRect(rect, paint);
    else
        m_canvas->drawRoundRect(rect, radius, radius, paint);
}

void skia_backend::draw_line(const dcmr::vec2 &from, const dcmr::vec2 &to,
                             const dcmr::color &col, float line_width) {
    SkPaint paint;
    paint.setColor(SkColorSetARGBMacro(col.a, col.r, col.g, col.b));
    paint.setStrokeWidth(line_width);
    m_canvas->drawLine(from.x(), from.y(), to.x(), to.y(), paint);
}

dcmr::vec2 skia_backend::get_image_extents(const std::string &src) {
    auto data = SkData::MakeFromFileName(src.c_str());
    auto image = SkImage::MakeFromEncoded(data);

    dcmr::vec2 extents{static_cast<float>(image->width()),
                       static_cast<float>(image->height())};

    return extents;
}

void skia_backend::draw_image(const std::string &src, const dcmr::vec2 &pos,
                              int width, int height) {
    auto data = SkData::MakeFromFileName(src.c_str());
    auto image = SkImage::MakeFromEncoded(data);
    SkPaint paint;

    SkRect dst = SkRect::MakeXYWH(pos.x(), pos.y(), width, height);

    m_canvas->drawImageRect(image, dst, &paint);
}

bool skia_backend::is_visited_uri(const std::string &uri) { return false; }

void skia_backend::set_clip_rect(const dcmr::rect &rectangle) {
    SkRect clip_rect = SkRect::MakeLTRB(
        rectangle.top_left().x(), rectangle.top_left().y(),
        rectangle.bottom_right().x(), rectangle.bottom_right().y());

    m_canvas->clipRect(clip_rect, SkRegion::kReplace_Op);
}

void skia_backend::save() { m_canvas->save(); }

void skia_backend::restore() { m_canvas->restore(); }

#include "skia_renderer.h"

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
    }

    extents string_extents(const std::string &text) const {
        SkRect bounds;
        m_paint.measureText(text.c_str(), text.length(), &bounds);
        return extents(bounds.width(), bounds.height());
    }

  private:
    sk_sp<SkTypeface> m_typeface;
    SkPaint m_paint;
};

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
                                font *fnt_in) {}

#include "skia_renderer.h"

#include "SkTypeface.h"

class skia_font : public font {};

font *skia_renderer::create_font(const std::string &family,
                                 const std::string &style, int size) {
    sk_sp<SkFontMgr> fmgr = SkFontMgr::RefDefault();

    int weight = SkFontStyle::kNormal_Weight;
    if (style == "Bold" || style == "BoldItalic")
        weight = SkFontStyle::kBold_Weight;

    SkFontStyle font_style{weight, SkFontStyle::kNormal_Width,
                           SkFontStyle::kUpright_Slant};

    SkTypeface *typeface = fmgr->matchFamilyStyle(family.c_str(), font_style);

    return nullptr;
}

extents skia_renderer::string_extents(const font *fnt,
                                      const std::string &string) {
    extents ext;
    return ext;
}

void skia_renderer::draw_string(const std::string &text, const position &pos,
                                font *fnt_in) {}

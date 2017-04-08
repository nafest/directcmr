#include "renderer.h"

#include <iostream>

renderer::renderer() {
    // set some default parameters
    set_string_param("font", "Arial");
    set_string_param("color", "#000000ff");
    set_float_param("margin_left", 0.0);
    set_float_param("margin_bottom", 0.0);
    set_float_param("margin_top", 0.0);

    set_string_param("code_block.font", "Menlo");
    set_float_param("code_block.margin_left", 10.f);
    set_float_param("code_block.margin_top", 5.f);
    set_float_param("code_block.margin_bottom", 5.f);

    set_float_param("list.margin_left", 15.0f);

    set_string_param("blockquote.color", "#606060ff");
    set_float_param("blockquote.margin_left", 10.f);
    set_float_param("blockquote.margin_top", 5.f);
    set_float_param("blockquote.margin_bottom", 5.f);

    set_float_param("table.border_width", 1.f);
}

font *renderer::font_for_style(const style &st) {
    auto search = m_cached_fonts.find(st);
    if (search != m_cached_fonts.end())
        return search->second;

    std::string style_str = "";
    if (st.get_strong())
        style_str += "Bold";
    if (st.get_emph())
        style_str += "Italic";

    if (style_str == "")
        style_str = "Normal";

    std::string family_str;
    if (st.get_code())
        family_str = get_string_param("code_block.font");
    else
        family_str = default_family();

    int size = default_size();
    if (st.get_heading_level() > 0)
        size = heading_size(st.get_heading_level());

    auto font = create_font(family_str, style_str, size);
    m_cached_fonts[st] = font;
    return font;
}

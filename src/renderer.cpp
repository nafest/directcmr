#include "renderer.h"

#include <iostream>

renderer::renderer() {
    // set some default parameters
    set_string_param("font", "Arial");
    set_string_param("color", "#000000ff");

    // for minimal spacing between elements:
    set_float_param("margin_top", 2.f);
    set_float_param("margin_bottom", 2.f);

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
    set_float_param("table_cell.margin", 2.f);
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

float renderer::get_side_margin(const std::string &element_name,
                                const std::string side) const noexcept {
    float mrgn = 0.f;

    // use them
    auto elem = m_float_params.find(element_name + ".margin_" + side);
    if (elem == m_float_params.end())
        elem = m_float_params.find(element_name + ".margin");
    if (elem == m_float_params.end())
        elem = m_float_params.find("margin_" + side);
    if (elem == m_float_params.end())
        elem = m_float_params.find("margin");
    if (elem != m_float_params.end())
        mrgn = elem->second;

    return mrgn;
}

elem_margin renderer::get_margin(const std::string &element_name) const
    noexcept {
    elem_margin mrgn(0.f, 0.f, 0.f, 0.f);

    mrgn.left = get_side_margin(element_name, "left");
    mrgn.top = get_side_margin(element_name, "top");
    mrgn.right = get_side_margin(element_name, "right");
    mrgn.bottom = get_side_margin(element_name, "bottom");

    return mrgn;
}
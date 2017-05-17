// cmark_renderer - a renderer for CommonMark
//
// backend.cpp - abstract base class for renderer backends
//
// Copyright (c) 2017 Stefan Winkler
// License: MIT License (for full license see LICENSE)

#include "backend.h"

#include <iostream>

cmr::backend::backend() {
    // set some default parameters
    set_string_param("font", "Arial");
    set_float_param("font_size", 14.f);
    set_string_param("color", "#000000ff");
    set_string_param("background_color", "#00000000"); // be transparent

    set_float_param("document.margin", 2.f);

    // for minimal spacing between elements:
    set_float_param("margin_top", 2.f);
    set_float_param("margin_bottom", 2.f);

    set_string_param("code.font", "Menlo");
    set_string_param("code.background_color", "#2b2b2bff");
    set_float_param("code.border_radius", 3.f);
    set_string_param("code.color", "#a9b7c6ff");
    set_float_param("code.margin_left", 2.f);
    set_float_param("code.margin_right", 2.f);

    set_string_param("code_block.font", "Menlo");
    set_string_param("code_block.color", "#a9b7c6ff");
    set_float_param("code_block.margin_left", 10.f);
    set_float_param("code_block.margin_top", 10.f);
    set_float_param("code_block.margin_bottom", 10.f);
    set_string_param("code_block.background_color", "#2b2b2bff");
    set_float_param("code_block.border_radius", 3.f);

    set_float_param("list.margin_left", 15.0f);

    set_string_param("blockquote.color", "#606060ff");
    set_float_param("blockquote.margin_left", 10.f);
    set_float_param("blockquote.margin_top", 5.f);
    set_float_param("blockquote.margin_bottom", 5.f);

    set_float_param("table.border_width", 1.f);
    set_float_param("table_cell.margin", 2.f);
}

cmr::font *cmr::backend::font_for_style(const cmr::style &st) {
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
    else if (st.get_inline_code())
        family_str = get_string_param("code.font");
    else
        family_str = get_string_param("font");

    int size = get_float_param("font_size");
    if (st.get_heading_level() > 0)
      for (int i = 6; i >= st.get_heading_level(); i--)
        size = static_cast<int>(size * 1.1f);

    auto font = create_font(family_str, style_str, size);
    m_cached_fonts[st] = font;
    return font;
}

float cmr::backend::get_side_margin(const std::string &element_name,
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

cmr::elem_margin
cmr::backend::get_margin(const std::string &element_name) const noexcept {
    cmr::elem_margin mrgn(0.f, 0.f, 0.f, 0.f);

    mrgn.left = get_side_margin(element_name, "left");
    mrgn.top = get_side_margin(element_name, "top");
    mrgn.right = get_side_margin(element_name, "right");
    mrgn.bottom = get_side_margin(element_name, "bottom");

    return mrgn;
}
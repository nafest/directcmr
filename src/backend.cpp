// directcmr - the CommonMark renderer you never wanted
//
// backend.cpp - abstract base class for renderer backends
//
// Copyright (c) 2017 Stefan Winkler
// License: MIT License (for full license see LICENSE)

#include "backend.h"

#include <iostream>

dcmr::font *dcmr::backend::font_for_style(const dcmr::style &st) {
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
        family_str = m_style_sheet.get_string_param("code_block.font");
    else if (st.get_inline_code())
        family_str = m_style_sheet.get_string_param("code.font");
    else
        family_str = m_style_sheet.get_string_param("font");

    int size = static_cast<int>(m_style_sheet.get_float_param("font_size"));
    if (st.get_heading_level() > 0)
        for (int i = 6; i >= st.get_heading_level(); i--)
            size = static_cast<int>(size * 1.1f);

    auto font = create_font(family_str, style_str, size);
    m_cached_fonts[st] = font;
    return font;
}

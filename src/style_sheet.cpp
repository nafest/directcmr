// directcmr - the CommonMark renderer you never wanted
//
// style_sheet.cpp - class holding the styling of a document
//                   loosely related to CSS
//
// Copyright (c) 2017 Stefan Winkler
// License: MIT License (for full license see LICENSE)

#include "style_sheet.h"
#include "utils.h"

dcmr::style_sheet::style_sheet() {
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

float dcmr::style_sheet::get_float_param(const std::string &param_name) const
    noexcept {
    auto elem = m_float_params.find(param_name);
    if (elem == m_float_params.end()) {
        auto dot_pos = param_name.find('.');
        if (dot_pos != std::string::npos) {
            return get_float_param(param_name.substr(dot_pos + 1));
        }
        return 0.f;
    }
    return elem->second;
}

void dcmr::style_sheet::set_float_param(const std::string &param_name,
                                        float value) noexcept {
    m_float_params[param_name] = value;
}

std::string
dcmr::style_sheet::get_string_param(const std::string &param_name) const
    noexcept {
    auto elem = m_string_params.find(param_name);
    if (elem == m_string_params.end()) {
        auto dot_pos = param_name.find('.');
        if (dot_pos != std::string::npos) {
            return get_string_param(param_name.substr(dot_pos + 1));
        }
        return "";
    }
    return elem->second;
}

void dcmr::style_sheet::set_string_param(const std::string &param_name,
                                         const std::string &value) noexcept {
    m_string_params[param_name] = value;
}

float dcmr::style_sheet::get_side_margin(const std::string &element_name,
                                         const std::string& side) const
    noexcept {
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

dcmr::elem_margin
dcmr::style_sheet::get_margin(const std::string &element_name) const noexcept {
    dcmr::elem_margin mrgn(0.f, 0.f, 0.f, 0.f);

    mrgn.left = get_side_margin(element_name, "left");
    mrgn.top = get_side_margin(element_name, "top");
    mrgn.right = get_side_margin(element_name, "right");
    mrgn.bottom = get_side_margin(element_name, "bottom");

    return mrgn;
}

std::string trim(const std::string &in) {
    // remove space at the front and the end of the input
    // string
    size_t start = 0;
    auto len = in.size();
    // first at the front
    while (start < len && in[start] == ' ')
        start++;
    auto end = in.find(' ', start + 1);

    if (end == std::string::npos) {
        if (in[len - 1] == '\n') {
            return in.substr(start, len - start - 1);
        }
        return in.substr(start);
    } 
        if (in[end] == '\n')
            end--;
        return in.substr(start, end - start);
    
}

std::pair<std::string, std::string>
dcmr::split_key_value(const std::string &key_val_string) {
    auto colon_pos = key_val_string.find(':');

    if (colon_pos == std::string::npos)
        return std::make_pair("", "");

    std::pair<std::string, std::string> out_pair;
    out_pair.first = trim(key_val_string.substr(0, colon_pos));
    out_pair.second =
        trim(key_val_string.substr(colon_pos + 1, std::string::npos));

    return out_pair;
};

// an easier way to set a lot of styling parameters
//
// pass a string with a key value pair in every line
// code_block.font: Consolas\n
// code_block.color: #a9b7c6ff\n
// code_block.margin: 2.f\n
void dcmr::style_sheet::set_params(const std::string &par_value_list) noexcept {
    for (auto &line : line_splitter(par_value_list)) {
        auto kv = dcmr::split_key_value(line);

        try {
            auto param = std::stof(kv.second);
            set_float_param(kv.first, param);
        } catch (std::invalid_argument) {
            set_string_param(kv.first, kv.second);
        }
    }
}

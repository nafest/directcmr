// directcmr - the CommonMark renderer you never wanted
//
// style_sheet.h - class holding the styling of a document
//                 loosely related to CSS
//
// Copyright (c) 2017 Stefan Winkler
// License: MIT License (for full license see LICENSE)

#pragma once

#include <map>
#include <string>

namespace dcmr {

// struct for margins (left, top, right, bottom)
struct elem_margin {
    elem_margin(float _left, float _top, float _right, float _bottom)
        : top(_top), left(_left), right(_right), bottom(_bottom) {}
    float vertical_margin() const { return top + bottom; }
    float horizontal_margin() const { return left + right; }
    float top, left, bottom, right;
};

class style_sheet {
  public:
    style_sheet();

    virtual float get_float_param(const std::string &param_name) const noexcept;

    virtual void set_float_param(const std::string &param_name,
                                 float value) noexcept;

    virtual std::string get_string_param(const std::string &param_name) const
        noexcept;

    virtual void set_string_param(const std::string &param_name,
                                  const std::string &value) noexcept;

    virtual elem_margin get_margin(const std::string &element_name) const
        noexcept;

    void set_params(const std::string &par_value_list) noexcept;

  private:
    float get_side_margin(const std::string &element_name,
                          const std::string side) const noexcept;

    std::map<std::string, float> m_float_params;
    std::map<std::string, std::string> m_string_params;
};

std::pair<std::string, std::string>
split_key_value(const std::string &key_val_string);
}

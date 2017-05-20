// directcmr - the CommonMark renderer you never wanted
//
// heading_element.h - class for headings
//
// Copyright (c) 2017 Stefan Winkler
// License: MIT License (for full license see LICENSE)

#pragma once

#include "leaf_block_element.h"

#include <iostream>

namespace dcmr {

class heading_element : public leaf_block_element {
  public:
    heading_element(int heading_level)
        : leaf_block_element(), m_heading_level(heading_level) {}

    virtual void propagate_style(style st) noexcept override {
        st.set_heading_level(m_heading_level);
        element::propagate_style(st);
    }

  private:
    int m_heading_level;
};
}

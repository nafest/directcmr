// directcmr - the CommonMark renderer you never wanted
//
// strong_element.h - class for strong spans
//
// Copyright (c) 2017 Stefan Winkler
// License: MIT License (for full license see LICENSE)

#pragma once

#include "emph_element.h"

namespace dcmr {

class strong_element : public emph_element {
  public:
    virtual void propagate_style(style st) noexcept override {
        st.set_strong(true);
        element::propagate_style(st);
    }
};
}

#pragma once

#include "emph_element.h"

class code_element : public emph_element {
  public:
    virtual void propagate_style(style st) noexcept override {
        st.set_code(true);
        element::propagate_style(st);
    }
};

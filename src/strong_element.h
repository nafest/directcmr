#pragma once

#include "emph_element.h"

class strong_element : public emph_element {
  public:
    virtual void propagate_style(style st) noexcept override {
        st.set_strong(true);
        element::propagate_style(st);
    }
};

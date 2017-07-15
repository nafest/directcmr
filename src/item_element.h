// directcmr - the CommonMark renderer you never wanted
//
// item_element.h - class for items in a list
//
// Copyright (c) 2017 Stefan Winkler
// License: MIT License (for full license see LICENSE)

#pragma once

#include "element.h"

namespace dcmr {

class item_element : public element {
  public:
    float layout(backend *bcknd, float width) override {
        auto margin = bcknd->get_style_sheet().get_margin("item");
        float offset = margin.top;
        for (auto child : m_children) {
            child->set_position(vec2(margin.left, offset));
            offset += child->layout(bcknd, width - margin.left);
        }
        return offset + margin.bottom;
    }
};
}

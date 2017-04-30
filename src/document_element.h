// cmark_renderer - a renderer for CommonMark
//
// document_element.h - root element of a CommonMark
//                      document tree
//
// Copyright (c) 2017 Stefan Winkler
// License: MIT License (for full license see LICENSE)

#pragma once

#include "element.h"

class document_element : public element {
  public:
    virtual float layout(renderer *rndr, float width) {
        auto margin = rndr->get_margin("document");
        float cumul_height = margin.top;
        for (auto child : m_children) {
            child->set_position({margin.left, cumul_height});
            cumul_height +=
                child->layout(rndr, width - margin.horizontal_margin());
        }

        return cumul_height + margin.top;
    }
};

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

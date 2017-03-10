#pragma once

#include "element.h"

class document_element : public element {
  public:
    virtual float layout(renderer *rndr, float width) {
        float cumul_height = 0;
        for (auto child : m_children) {
            child->set_position({0, cumul_height});
            cumul_height += child->layout(rndr, width);
        }

        return cumul_height;
    }
};

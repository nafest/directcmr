#pragma once

#include "element.h"

class list_element : public element {
  public:
    virtual float layout(renderer *rndr, float width) override;
};

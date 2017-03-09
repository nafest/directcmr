#pragma once

#include "element.h"

class list_element : public element {
  public:
    virtual float layout(renderer *rndr, float width) override;
    virtual void render(renderer *rndr, vec2 pos = {0, 0}) override;
};

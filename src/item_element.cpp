#include "item_element.h"

#include <cassert>
float item_element::layout(renderer *rndr, float width) {
    assert(m_children.size() == 1);
    return m_children[0]->layout(rndr,width);
}

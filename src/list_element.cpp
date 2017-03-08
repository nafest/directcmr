#include "list_element.h"

float list_element::layout(renderer *rndr, float width) {
    auto margin = rndr->get_float_param("list_margin");
    float offset = 0.f;
    for (auto child : m_children) {
        child->set_position(vec2(margin, offset));
        offset += child->layout(rndr, width - margin);
    }
    return offset;
}

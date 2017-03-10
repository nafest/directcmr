#pragma once

#include "element.h"

class list_element : public element {
  public:
    float layout(renderer *rndr, float width) override {
        auto margin = rndr->get_float_param("list_margin");
        float offset = 0.f;
        for (auto child : m_children) {
            child->set_position(vec2(margin, offset));
            offset += child->layout(rndr, width - margin);
        }
        return offset;
    }

    void render(renderer *rndr, vec2 pos) override {
        for (auto child : m_children) {
            auto child_pos = child->get_position();
            child_pos = child_pos + pos + m_pos;
            vec2 marker_top_left(child_pos.x() -
                                     rndr->get_float_param("list_margin"),
                                 child_pos.y());
            vec2 marker_bottom_right(
                child_pos.x(),
                child_pos.y() + child->get_font(rndr)->get_line_height());

            rect marker_rect(marker_top_left, marker_bottom_right);
            rndr->draw_list_marker(marker_rect);
            child->render(rndr, pos + m_pos);
        }
    }
};

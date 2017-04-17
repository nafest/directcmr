#pragma once

#include "element.h"

#include <cmath>
#include <iostream>
#include <strstream>

class list_element : public element {
  public:
    enum class list_element_type { ordered, unordered };

    list_element(list_element_type type) : m_type(type) {}

    // return the margin required to render this list
    // On default this is given by list.margin_left
    // However if this value is too small to render
    // the ordinals, return a sufficient value
    float element_margin(renderer *rndr) {
        if (m_type == list_element_type::unordered)
            return rndr->get_margin("list").left;
        else {
            auto num_char =
                static_cast<float>(std::ceil(std::log10(m_children.size())));
            return std::max<float>(rndr->get_margin("list").left,
                                   get_font(rndr)->get_x_width() *
                                       (num_char + 1.f));
        }
    }

    float layout(renderer *rndr, float width) override {
        auto margin_left = element_margin(rndr);
        auto margin = rndr->get_margin("list");
        float offset = margin.top;
        for (auto child : m_children) {
            child->set_position(vec2(margin_left, offset));
            offset += child->layout(rndr, width - margin_left);
        }
        return offset + margin.bottom;
    }

    void render(renderer *rndr, vec2 pos) override {
        int cnt = 1;
        auto num_char =
            static_cast<float>(std::ceil(std::log10(m_children.size())));
        for (auto child : m_children) {
            auto child_pos = child->get_position();
            child_pos = child_pos + pos + m_pos;
            vec2 marker_top_left(child_pos.x() - rndr->get_margin("list").left,
                                 child_pos.y());
            vec2 marker_bottom_right(
                child_pos.x(),
                child_pos.y() + child->get_font(rndr)->get_line_height());

            rect marker_rect(marker_top_left, marker_bottom_right);
            if (m_type == list_element_type::unordered)
                rndr->draw_list_marker(marker_rect);
            else {
                marker_top_left.y() =
                    child_pos.y() + child->get_font(rndr)->get_ascent();
                marker_top_left.x() =
                    child_pos.x() -
                    (num_char + 0.5f) * child->get_font(rndr)->get_x_width();
                std::stringstream order_stream;
                order_stream << cnt << ".";
                std::string order = order_stream.str();
                rndr->draw_string(order, marker_top_left, get_font(rndr),
                                  get_color(rndr));
                cnt++;
            }
            child->render(rndr, pos + m_pos);
        }
    }

  private:
    list_element_type m_type;
};

// directcmr - the CommonMark renderer you never wanted
//
// list_element.h - class for ordered and unordered lists
//
// Copyright (c) 2017 Stefan Winkler
// License: MIT License (for full license see LICENSE)

#pragma once

#include "element.h"

#include <cmath>
#include <iostream>

namespace dcmr {

class list_element : public element {
  public:
    enum class list_element_type { ordered, unordered };

    list_element(list_element_type type) : m_type(type) {}

    // return the margin required to render this list
    // On default this is given by list.margin_left
    // However if this value is too small to render
    // the ordinals, return a sufficient value
    float element_margin(backend *bcknd) {
        auto ss = bcknd->get_style_sheet();
        if (m_type == list_element_type::unordered)
            return ss.get_margin("list").left;
        else {
            float num_char = 1.f;
            if (m_children.size() > 1)
                num_char = static_cast<float>(
                    std::ceil(std::log10(m_children.size())));
            return std::max<float>(ss.get_margin("list").left,
                                   get_font(bcknd)->get_x_width() *
                                       (num_char + 1.f));
        }
    }

    float layout(backend *bcknd, float width) override {
        auto margin_left = element_margin(bcknd);
        auto margin = bcknd->get_style_sheet().get_margin("list");
        float offset = margin.top;
        for (auto child : m_children) {
            child->set_position(vec2(margin_left, offset));
            offset += child->layout(bcknd, width - margin_left);
        }
        return offset + margin.bottom;
    }

    void render(backend *bcknd, vec2 pos) override {
        auto ss = bcknd->get_style_sheet();
        int cnt = 1;
        float num_char = 1.f;
        if (m_children.size() > 1)
            num_char =
                static_cast<float>(std::ceil(std::log10(m_children.size())));
        for (auto child : m_children) {
            auto child_pos = child->get_position();
            child_pos = child_pos + pos + m_rect.top_left();

            // take the margin of the first drawn element into account
            // this should span to levels:
            // list -> item -> paragraph
            float first_elem_margin_top = 0.f;
            if (child->children().size() > 0)
                first_elem_margin_top =
                    ss.get_margin(child->get_type()).top +
                    ss.get_margin(child->children()[0]->get_type()).top;

            vec2 marker_top_left(child_pos.x() - ss.get_margin("list").left,
                                 child_pos.y() + first_elem_margin_top);
            vec2 marker_bottom_right(
                child_pos.x(), child_pos.y() + first_elem_margin_top +
                                   child->get_font(bcknd)->get_line_height());

            rect marker_rect(marker_top_left, marker_bottom_right);
            if (m_type == list_element_type::unordered)
                bcknd->draw_list_marker(marker_rect, color(0, 0, 0, 255));
            else {
                // in order to render the ordinal correctly, to top margin of
                // the child element has
                // to be taken into account
                marker_top_left.y() = child_pos.y() +
                                      child->get_font(bcknd)->get_ascent() +
                                      first_elem_margin_top;
                marker_top_left.x() =
                    child_pos.x() -
                    (num_char + 0.5f) * child->get_font(bcknd)->get_x_width();
                std::stringstream order_stream;
                order_stream << cnt << ".";
                std::string order = order_stream.str();
                bcknd->draw_string(order, marker_top_left, get_font(bcknd),
                                   get_color(bcknd));
                cnt++;
            }
            child->render(bcknd, pos + m_rect.top_left());
        }
    }

  private:
    list_element_type m_type;
};
}
#pragma once

#include "element.h"

class blockquote_element : public element {
  public:
    virtual float layout(renderer *rndr, float width) override {
        width -= rndr->get_float_param("blockquote.margin_left");

        m_height = element::layout(rndr, width);

        m_height += rndr->get_float_param("blockquote.margin_top") +
                    rndr->get_float_param("blockquote.margin_bottom");

        return m_height;
    }

    virtual void render(renderer *rndr, vec2 pos = {0, 0}) override {
        vec2 margin(rndr->get_float_param("blockquote.margin_left"),
                    rndr->get_float_param("blockquote.margin_top"));

        rect bq_marker(pos + m_pos + vec2(0, 0),
                       pos + m_pos + vec2(5, m_height));

        rndr->draw_rect(bq_marker);
        for (auto child : m_children)
            child->render(rndr, pos + m_pos + margin);
    }
    virtual void propagate_style(style st) noexcept override {
        st.set_blockquote(true);
        element::propagate_style(st);
    }

  public:
    float m_height;
};

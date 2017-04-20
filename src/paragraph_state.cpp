#include "paragraph_state.h"
#include "element.h"

void paragraph_state::set_base_line() {
    float max_dist_to_base_line = 0.f;

    for (auto elem : m_line_elements) {
        max_dist_to_base_line = std::max<float>(max_dist_to_base_line, elem->get_top_to_base_line());
    }

    float max_correction = 0.f;
    for (auto elem : m_line_elements) {
        auto correction = max_dist_to_base_line - elem->get_top_to_base_line();
        max_correction = std::max<float>(max_correction, correction);
        auto pos = elem->get_position();
        pos.y() += correction;
        elem->set_position(pos);
    }

    m_posy += max_correction;
}


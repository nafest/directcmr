// directcmr - the CommonMark renderer you never wanted
//
// table_element.h - contains all classes to cope with
//                   GitHubs table extension
//
// Copyright (c) 2017 Stefan Winkler
// License: MIT License (for full license see LICENSE)

#pragma once

#include "leaf_block_element.h"

#include <numeric>

namespace dcmr {
// Layouting tables is a bit more complex and should fulfil the following
// constraints:
// - The width of the columns should adapt to the contents of the columns
// - A minimal width (width of the largest word in a column) should be
//   respected if possible
// - spacing between the columns/cells should be added

class table_element : public element {
  private:
    int num_column() const {
        // get the first child, which should either be a table_header or
        // a table_row;
        if (m_children.size() == 0)
            return 0;
        return static_cast<int>(m_children[0]->children().size());
    }

    std::vector<float> min_col_widths(backend *bcknd) const {
        std::vector<float> col_widths(num_column(), 0.f);

        for (auto child : m_children) {
            for (int i = 0; i < child->children().size(); i++) {
                col_widths[i] = std::max<float>(
                    col_widths[i], child->children()[i]->min_width(bcknd));
            }
        }

        return col_widths;
    }

    std::vector<float> preferred_col_widths(backend *bcknd) const {
        std::vector<float> col_widths(num_column(), 0.f);

        for (auto child : m_children) {
            for (int i = 0; i < child->children().size(); i++) {
                col_widths[i] = std::max<float>(
                    col_widths[i],
                    child->children()[i]->preferred_width(bcknd));
            }
        }

        return col_widths;
    }

  public:
    static std::vector<float> distribute_width(float width, int num,
                                               std::vector<float> min_widths) {
        std::vector<float> widths(num);

        // assign all elements the minimum width and equally
        // distribute the remaining space
        auto remaining =
            width - std::accumulate(min_widths.begin(), min_widths.end(), 0.f);
        for (int i = 0; i < num; i++)
            widths[i] = min_widths[i] + remaining / num;

        return widths;
    }

    virtual float layout(backend *bcknd, float width) {
        auto ss = bcknd->get_style_sheet();
        // styling options required for layouting the table
        auto border_width = ss.get_float_param("table.border_width");

        auto num_col = num_column();
        float spacing_width = (num_col + 1) * border_width;

        // estimate the width used to render the table. If the sum of the
        // preferred widths is smaller than width use this value, otherwise
        // use width
        auto preferred_widths = preferred_col_widths(bcknd);
        float preferred_table_width =
            spacing_width + std::accumulate(preferred_widths.begin(),
                                            preferred_widths.end(), 0.f);
        std::vector<float> column_widths(num_col);
        if (preferred_table_width < width) {
            width = preferred_table_width;
            column_widths = preferred_widths;
        } else {
            column_widths = distribute_width(width - spacing_width, num_col,
                                             min_col_widths(bcknd));
        }

        // given the column widths it is now possible to layout all rows
        // pre-compute the horizontal offset of all columns;
        m_grid_col.resize(num_col + 1);
        m_grid_col[0] = 0.5f * border_width;
        for (int i = 1; i < num_col + 1; i++)
            m_grid_col[i] =
                border_width + column_widths[i - 1] + m_grid_col[i - 1];

        float height = ss.get_margin("table").top + border_width;
        m_grid_row.push_back(height - 0.5f * border_width);
        for (auto row : m_children) {
            float row_height = 0.f;
            for (int i = 0; i < num_col; i++) {
                auto col = row->children()[i];
                col->set_position(
                    vec2(m_grid_col[i] + 0.5f * border_width, height));
                row_height = std::max<float>(
                    row_height, col->layout(bcknd, column_widths[i]));
            }
            height += row_height + border_width;
            m_grid_row.push_back(height - 0.5f * border_width);
        }

        return height + ss.get_margin("table").bottom;
    }

    virtual void render(backend *bcknd, vec2 pos = {0, 0}) {
        auto num_col = num_column();
        auto border_width =
            bcknd->get_style_sheet().get_float_param("table.border_width");

        // render the horizontal lines;
        float width = m_grid_col.back() + 0.5f * border_width;
        float left = m_grid_col.front() - 0.5f * border_width;
        for (auto y : m_grid_row)
            bcknd->draw_line(vec2(left, y) + pos + m_rect.top_left(),
                             vec2(width, y) + pos + m_rect.top_left(),
                             color(0, 0, 0, 255), border_width);

        // render the vertical lines;
        float height = m_grid_row.back();
        float top = m_grid_row.front() - 0.5f * border_width;
        for (auto x : m_grid_col)
            bcknd->draw_line(vec2(x, top) + pos + m_rect.top_left(),
                             vec2(x, height) + pos + m_rect.top_left(),
                             color(0, 0, 0, 255), border_width);

        for (auto row : m_children) {
            for (int i = 0; i < num_col; i++) {
                auto cell = row->children()[i];
                cell->render(bcknd, pos + m_rect.top_left());
            }
        }
    }

  private:
    // store the position of the grid in layout() for later use
    // in render()
    std::vector<float> m_grid_col;
    std::vector<float> m_grid_row;
};

class table_header_element : public element {};
class table_row_element : public element {};
class table_cell_element : public leaf_block_element {};
}

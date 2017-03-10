#include "document.h"
#include "cmark.h"
#include "document_element.h"
#include "element.h"
#include "emph_element.h"
#include "heading_element.h"
#include "item_element.h"
#include "list_element.h"
#include "paragraph_element.h"
#include "strong_element.h"
#include "text_element.h"

#include <iostream>
#include <vector>

std::vector<element *> transform_children(cmark_node *node) {
    std::vector<element *> child_elements;
    auto child = cmark_node_first_child(node);
    while (child) {
        element *elem;
        switch (cmark_node_get_type(child)) {
        case CMARK_NODE_PARAGRAPH:
            elem = new paragraph_element();
            break;
        case CMARK_NODE_TEXT:
            elem = new text_element();
            elem->set_literal(cmark_node_get_literal(child));
            break;
        case CMARK_NODE_EMPH:
            elem = new emph_element();
            break;
        case CMARK_NODE_STRONG:
            elem = new strong_element();
            break;
        case CMARK_NODE_HEADING:
            elem = new heading_element(cmark_node_get_heading_level(child));
            break;
        case CMARK_NODE_LIST:
            elem = new list_element();
            break;
        case CMARK_NODE_ITEM:
            elem = new item_element();
            break;
        default:
            elem = new element();
            std::cout << "unsupported element kind "
                      << cmark_node_get_type_string(child) << std::endl;
        }
        child_elements.push_back(elem);
        elem->set_children(transform_children(child));
        elem->set_type(cmark_node_get_type_string(child));

        child = cmark_node_next(child);
    }
    return child_elements;
}

document document::fromString(const std::string &string) {
    cmark_node *document_root = cmark_parse_document(
        string.c_str(), string.length(), CMARK_OPT_DEFAULT);

    element *elem = new document_element();

    auto child_elements = transform_children(document_root);
    elem->set_children(child_elements);

    cmark_node_free(document_root);

    // propagate styling information down the tree, starting
    // with the default style
    elem->propagate_style();
    return document(elem);
}

document document::fromFile(const std::string &file_name) {
    return document::fromString("### Heading *bold* Heading\nHello *world*");
}

int document::layout(int width) {
    m_layout_width = width;
    return m_root_element->layout(m_renderer, width);
}

void document::render(vec2 origin, int height) {
    m_renderer->prepare_canvas(m_layout_width, height);
    m_root_element->render(m_renderer, origin);
}

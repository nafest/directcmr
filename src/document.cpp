#include "document.h"
#include "cmark.h"
#include "cmark_extension_api.h"
#include "code_block_element.h"
#include "document_element.h"
#include "element.h"
#include "emph_element.h"
#include "heading_element.h"
#include "item_element.h"
#include "list_element.h"
#include "paragraph_element.h"
#include "registry.h"
#include "strong_element.h"
#include "text_element.h"

#include "../extensions/core-extensions.h"

#include <iostream>
#include <vector>

std::vector<element *> transform_children(cmark_node *node, bool be_verbose) {
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
        case CMARK_NODE_LIST: {
            auto type = cmark_node_get_list_type(child) == CMARK_ORDERED_LIST
                            ? list_element::list_element_type::ordered
                            : list_element::list_element_type::unordered;
            elem = new list_element(type);
            break;
        }
        case CMARK_NODE_ITEM:
            elem = new item_element();
            break;
        case CMARK_NODE_CODE_BLOCK: {
            elem = new code_block_element();
            std::string literal = cmark_node_get_literal(child);
            // code blocks seem all to be ended with an extra
            // line break, remove it.
            literal.pop_back();
            elem->set_literal(literal);
            break;
        }
        default:
            elem = new element();
            std::cout << "unsupported element kind "
                      << cmark_node_get_type_string(child) << std::endl;
        }
        if (be_verbose) {

            std::cout << "add: " << cmark_node_get_type_string(child);
            if (cmark_node_get_literal(child))
                std::cout << " " << cmark_node_get_literal(child);
            std::cout << std::endl;
        }
        child_elements.push_back(elem);
        elem->set_children(transform_children(child, be_verbose));
        elem->set_type(cmark_node_get_type_string(child));

        child = cmark_node_next(child);
    }
    return child_elements;
}

document document::fromString(const std::string &string, bool be_verbose) {
    cmark_register_plugin(core_extensions_registration);
    int options = CMARK_OPT_DEFAULT;
#if DEBUG
    auto parser = cmark_parser_new(options);
#else
    auto parser =
        cmark_parser_new_with_mem(options, cmark_get_arena_mem_allocator());
#endif
    auto syntax_extension = cmark_find_syntax_extension("table");
    if (!syntax_extension) {
        fprintf(stderr, "Unknown extension table\n");
    }
    cmark_parser_attach_syntax_extension(parser, syntax_extension);
    cmark_parser_feed(parser, string.c_str(), string.length());
    auto document_root = cmark_parser_finish(parser);

    element *elem = new document_element();

    auto child_elements = transform_children(document_root, be_verbose);
    elem->set_children(child_elements);

#if DEBUG
    cmark_parser_free(parser);

    cmark_node_free(document_root);
#else
    cmark_arena_reset();
#endif
    cmark_release_plugins();

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

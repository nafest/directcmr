// cmark_renderer - a renderer for CommonMark
//
// document.cpp - implementation of the document class
//
// Copyright (c) 2017 Stefan Winkler
// License: MIT License (for full license see LICENSE)

#include "cmark.h"
#include "cmark_extension_api.h"

#include "blockquote_element.h"
#include "code_block_element.h"
#include "code_element.h"
#include "document.h"
#include "document_element.h"
#include "element.h"
#include "emph_element.h"
#include "heading_element.h"
#include "image_element.h"
#include "item_element.h"
#include "list_element.h"
#include "paragraph_element.h"
#include "registry.h"
#include "strong_element.h"
#include "table_element.h"
#include "text_element.h"
#include "thematic_break_element.h"

#include "../extensions/core-extensions.h"

#include <fstream>
#include <iostream>
#include <vector>

std::vector<element *> transform_children(cmark_node *node, bool be_verbose) {
    std::vector<element *> child_elements;
    auto child = cmark_node_first_child(node);
    bool softbreak_discarded = false;
    while (child) {
        element *elem;
        switch (cmark_node_get_type(child)) {
        case CMARK_NODE_PARAGRAPH:
            elem = new paragraph_element();
            break;
        case CMARK_NODE_TEXT:
            // do not separate subsequent text elements (because cmark returns
            // multiple text elements if a word contains an underscore, e.g.)
            if (child_elements.size() > 0 &&
                child_elements.back()->get_type() == "text" &&
                !softbreak_discarded) {
                // if the previously added element was already a text element
                // fuse this element with the last one
                auto literal = child_elements.back()->get_literal();
                child_elements.back()->set_literal(
                    literal + cmark_node_get_literal(child));
                elem = nullptr;
            } else {
                elem = new text_element();
                elem->set_literal(cmark_node_get_literal(child));
            }
            softbreak_discarded = false;
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
        case CMARK_NODE_CODE:
            elem = new code_element();
            if (cmark_node_get_literal(child)) {
                elem->children().push_back(
                    new text_element(cmark_node_get_literal(child)));
            }
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
        case CMARK_NODE_BLOCK_QUOTE:
            elem = new blockquote_element();
            break;
        case CMARK_NODE_IMAGE:
            elem = new image_element(cmark_node_get_url(child));
            break;
        case CMARK_NODE_THEMATIC_BREAK:
            elem = new thematic_break_element();
            break;
        case CMARK_NODE_HTML_BLOCK:
        case CMARK_NODE_HTML_INLINE:
            // ignore HTML block since cmark_renderer is no
            // HTML rendering engine
            break;
        case CMARK_NODE_SOFTBREAK:
            // since we don't output HTML, softbreaks can be ignored
            elem = nullptr;
            softbreak_discarded = true;
            break;
        default: {
            auto type_string = cmark_node_get_type_string(child);
            // elements created by cmark extensions such as tables
            // cannot be identified with a type constant, we
            // have to use the type string here
            if (!strcmp(type_string, "table")) {
                elem = new table_element();
            } else if (!strcmp(type_string, "table_header")) {
                elem = new table_header_element();
            } else if (!strcmp(type_string, "table_row")) {
                elem = new table_row_element();
            } else if (!strcmp(type_string, "table_cell")) {
                elem = new table_cell_element();
            } else {
                elem = new element();
                std::cout << "unsupported element kind "
                          << cmark_node_get_type_string(child) << std::endl;
            }
        }
        }
        if (be_verbose) {
            std::cout << "add: " << cmark_node_get_type_string(child);
            if (cmark_node_get_literal(child))
                std::cout << " " << cmark_node_get_literal(child);
            std::cout << std::endl;
        }
        if (elem) {
            child_elements.push_back(elem);
            elem->append_children(transform_children(child, be_verbose));
            elem->set_type(cmark_node_get_type_string(child));
        }

        child = cmark_node_next(child);
    }
    return child_elements;
}

cmark_parser *init_parser() {
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

    return parser;
}

void destroy_parser(cmark_parser *parser) {
#if DEBUG
    cmark_parser_free(parser);

    cmark_node_free(document_root);
#else
    cmark_arena_reset();
#endif
    cmark_release_plugins();
}

document document::from_string(const std::string &string, bool be_verbose) {
    auto parser = init_parser();

    cmark_parser_feed(parser, string.c_str(), string.length());
    auto document_root = cmark_parser_finish(parser);
    element *elem = new document_element();
    auto child_elements = transform_children(document_root, be_verbose);

    destroy_parser(parser);
    elem->append_children(child_elements);

    // propagate styling information down the tree, starting
    // with the default style
    elem->propagate_style();
    return document(elem);
}

document document::from_file(const std::string &file_name, bool be_verbose) {
    std::ifstream in_file(file_name);

    if (!in_file) {
        std::cerr << "Could not open " << file_name << "." << std::endl;
        return document(new document_element());
    }

    auto parser = init_parser();

    std::string line;
    while (std::getline(in_file, line)) {
        line += '\n';
        cmark_parser_feed(parser, line.c_str(), line.length());
    }

    auto document_root = cmark_parser_finish(parser);
    element *elem = new document_element();
    auto child_elements = transform_children(document_root, be_verbose);
    destroy_parser(parser);
    elem->append_children(child_elements);

    // propagate styling information down the tree, starting
    // with the default style
    elem->propagate_style();
    return document(elem);
}

int document::layout(int width) {
    m_layout_width = width;
    return static_cast<int>(std::ceil(
        m_root_element->layout(m_renderer, static_cast<float>(width))));
}

void document::render(vec2 origin, int height) {
    m_renderer->prepare_canvas(m_layout_width, height);
    m_root_element->render(m_renderer, origin);
}

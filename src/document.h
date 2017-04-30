// cmark_renderer - a renderer for CommonMark
//
// document.h - central class, that encapsulates the
//              element tree of a CommonMark document
//
// Copyright (c) 2017 Stefan Winkler
// License: MIT License (for full license see LICENSE)

#include "element.h"
#include "renderer.h"

#include <memory>
#include <string>

// the document class encapsulates the tree of nodes
// representing the markdown document
class document {
  public:
    static document from_file(const std::string &file_name, bool be_verbose = false);
    static document from_string(const std::string &string, bool be_verbose = false);

    void set_renderer(renderer *renderer) { m_renderer = renderer; }
    // layout the common mark document to a canvas with
    // a given maxium width. returns the height required to
    // render the document
    int layout(int width);

    void render(vec2 origin, int height);

    element *get_root_element() const noexcept { return m_root_element.get(); }

  private:
    document(element *element) : m_root_element(element), m_renderer(nullptr){};
    std::unique_ptr<element> m_root_element;
    renderer *m_renderer; // referenced renderer implementation,
                          // the document does not own the renderer
    int m_layout_width;
};

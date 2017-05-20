// directcmr - the CommonMark renderer you never wanted
//
// document.h - central class, that encapsulates the
//              element tree of a CommonMark document
//
// Copyright (c) 2017 Stefan Winkler
// License: MIT License (for full license see LICENSE)

#include "element.h"
#include "backend.h"

#include <memory>
#include <string>

namespace dcmr {

// the document class encapsulates the tree of nodes
// representing the markdown document
class document {
  public:
    static document from_file(const std::string &file_name,
                              bool be_verbose = false);
    static document from_string(const std::string &string,
                                bool be_verbose = false);

    void set_backend(backend *bcknd) { m_backend = bcknd; }

    // layout the CommonMark document to a canvas with
    // a given maximum width. returns the height required to
    // render the document
    int layout(int width);

    // render the document to the canvas at the given origin
    // and a maximum height. Everything exceeding height is
    // clipped (as long as the renderer) supports this
    void render(vec2 origin, int height);

    element *get_root_element() const noexcept { return m_root_element.get(); }

  private:
    document(element *element) : m_root_element(element), m_backend(nullptr){};
    std::unique_ptr<element> m_root_element;
    backend *m_backend; // referenced renderer implementation,
    // the document does not own the renderer
    int m_layout_width;
};
}

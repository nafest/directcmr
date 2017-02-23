#include "element.h"
#include "renderer.h"

#include <memory>
#include <string>

// the document class encapsulates the tree of nodes
// representing the markdown document
class document {
  public:
    static document fromFile(const std::string &file_name);
    static document fromString(const std::string &string);

    void set_renderer(renderer *renderer) { m_renderer = renderer; }
    // layout the common mark document to a canvas with
    // a given maxium width. returns the height required to
    // render the document
    int layout(int width);

    element *get_root_element() const noexcept { return m_root_element.get(); }

  private:
    document(element *element) : m_root_element(element), m_renderer(nullptr){};
    std::unique_ptr<element> m_root_element;
    renderer *m_renderer; // referenced renderer implementation,
                          // the document does not own the renderer
};

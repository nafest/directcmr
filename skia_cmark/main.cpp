#include "document.h"

#include "skia_renderer.h"

#include <iostream>

int main(int argc, const char *argv[]) {
    skia_renderer rndr;

    rndr.set_float_param("list_margin", 10.0);
    // document d = document::fromString("# Header\n## Level2\nHello **strong**
    // *world*\n- Item1\n- Item2\n\nblub\n    Hello to some code();\n
    // return;", true);
    document d = document::fromString(
        "# Header\n## Level2\nHello **strong** "
        "*world* `void main() {}`\n\n    Hello to some code(); main() {}\n  "
        "  return;\nback to paragraph\n1. List Item 1\n2. List Item 2\n\n> This is a\n> Block quote\n\n>     with some code();",
        true);
    // document d = document::fromString("    Hello to some code();");
    d.set_renderer(&rndr);
    std::cout << "layout height: " << d.layout(400) << std::endl;
    d.render(vec2(0, 0), 400);
    rndr.dump_canvas("dump.png");
    return 0;
}

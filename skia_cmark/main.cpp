#include "document.h"

#include "skia_renderer.h"

#include <iostream>

int main(int argc, const char *argv[]) {
    if (argc < 2) {
        std::cout << "no file given." << std::endl;
        return 1;
    }
    skia_renderer rndr;

    rndr.set_float_param("list_margin", 10.0);
    document d = document::from_file(argv[1], true);
    d.set_renderer(&rndr);
    std::cout << "layout height: " << d.layout(600) << std::endl;
    d.render(vec2(0, 0), 400);
    rndr.dump_canvas("dump.png");
    return 0;
}

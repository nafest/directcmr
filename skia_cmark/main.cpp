#include "document.h"

#include "skia_renderer.h"

#include <iostream>

int main(int argc, const char *argv[]) {
    skia_renderer rndr;
    rndr.set_float_param("list_margin", 10.0);
    document d = document::fromString("Hello **strong** *world*\n- Item1\n- Item2");
    d.set_renderer(&rndr);
    std::cout << "layout height: " << d.layout(100) << std::endl;
    d.render(vec2(0, 0), 400);
    rndr.dump_canvas("dump.png");
    return 0;
}

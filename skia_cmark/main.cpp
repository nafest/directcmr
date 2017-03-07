#include "document.h"

#include "skia_renderer.h"

#include <iostream>

int main(int argc, const char *argv[]) {
    skia_renderer rndr;
    document d = document::fromString("Hello **strong** *world*");
    d.set_renderer(&rndr);
    std::cout << "layout height: " << d.layout(100) << std::endl;
    d.render(position(0, 0), 400);
    rndr.dump_canvas("dump.png");
    return 0;
}

#include "document.h"

#include "skia_renderer.h"

#include <iostream>

int main(int argc, const char *argv[]) {
    skia_renderer rndr;
    document d = document::fromString("Hello *world*");
    d.set_renderer(&rndr);
    std::cout << "layout height: " << d.layout(100) << std::endl;
    d.render(400);
    rndr.dump_canvas("dump.png");
    return 0;
}

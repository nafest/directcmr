#include "document.h"

#include "skia_renderer.h"

int main(int argc, const char *argv[]) {
    skia_renderer rndr;
    document d = document::fromString("Hello *world*");
    d.set_renderer(&rndr);
    d.layout(100);
    d.render();

    return 0;
}

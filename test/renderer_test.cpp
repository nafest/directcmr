#include "gtest/gtest.h"

#include "document.h"
#include "fake_renderer.h"


TEST(Renderer, RenderSimple) {
    fake_renderer rndr;
    document d = document::fromString("Hello *world*");
    d.set_renderer(&rndr);
    d.layout(100);
    d.render();
}

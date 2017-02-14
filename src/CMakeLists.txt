add_library(cmark_renderer STATIC renderer.cpp)

target_link_libraries(cmark_renderer libcmark)

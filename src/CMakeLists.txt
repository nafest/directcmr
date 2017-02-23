add_library(cmark_renderer STATIC 
    document.cpp document.h
    renderer.cpp
    element.cpp element.h
    utils.cpp utils.h)
get_target_property(CMARK_SOURCE libcmark SOURCE_DIR)
get_target_property(CMARK_LOCATION libcmark BINARY_DIR)
target_link_libraries(cmark_renderer libcmark)
target_include_directories(cmark_renderer PRIVATE ${CMARK_SOURCE} ${CMARK_LOCATION})

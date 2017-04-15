project(cmark_renderer_library LANGUAGES CXX)

# since skia and googletest use the static runtime on default, use
# the static runtime as well for cmark_renderer
if(WIN32)
    string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
    string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
endif()

# do not tolerate warnings
if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
endif()

add_library(cmark_renderer STATIC
    document.cpp document.h
    renderer.cpp
    element.h
    item_element.h
    list_element.h
    utils.cpp utils.h)
get_target_property(CMARK_SOURCE libcmark SOURCE_DIR)
get_target_property(CMARK_LOCATION libcmark BINARY_DIR)
target_link_libraries(cmark_renderer libcmark_static libcmarkextensions_static)

# set compile definitions for static linkage of cmark
target_compile_definitions(cmark_renderer PRIVATE CMARK_STATIC_DEFINE CMARKEXTENSIONS_STATIC_DEFINE)
target_include_directories(cmark_renderer PRIVATE
    ${CMARK_SOURCE}
    ${CMARK_LOCATION}
    ${CMAKE_BINARY_DIR}/thirdparty/cmark/src
    ${CMAKE_BINARY_DIR}/thirdparty/cmark/extensions)


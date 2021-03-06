project(directcmr_library LANGUAGES CXX)

# since skia and googletest use the static runtime on default, use
# the static runtime as well for directcmr
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

add_library(directcmr STATIC
    blockquote_element.h
    code_block_element.h
    code_element.h
    document.cpp document.h
    document_element.h
    element.h
    emph_element.h
    geom.h
    heading_element.h
    item_element.h
    leaf_block_element.h
    list_element.h
    paragraph_element.h
    paragraph_state.h
    backend.cpp backend.h
    strong_element.h
    style.h
    table_element.h
    text_element.h
    utils.cpp utils.h image_element.h paragraph_state.cpp thematic_break_element.h link_element.h linebreak_element.h style_sheet.cpp style_sheet.h)
get_target_property(CMARK_SOURCE libcmark-gfm SOURCE_DIR)
get_target_property(CMARK_LOCATION libcmark-gfm BINARY_DIR)
target_link_libraries(directcmr libcmark-gfm_static libcmark-gfmextensions_static)

# set compile definitions for static linkage of cmark
target_compile_definitions(directcmr PRIVATE CMARK_STATIC_DEFINE CMARKEXTENSIONS_STATIC_DEFINE)
target_include_directories(directcmr PRIVATE
    ${CMARK_SOURCE}
    ${CMARK_LOCATION}
    ${CMAKE_BINARY_DIR}/thirdparty/cmark/src
    ${CMAKE_BINARY_DIR}/thirdparty/cmark/extensions)

if(WITH_COVERAGE)
    set_property(TARGET directcmr
        PROPERTY COMPILE_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
endif()
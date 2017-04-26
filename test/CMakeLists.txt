project(cmark_renderer_test LANGUAGES CXX)

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

add_executable(cmark_renderer_test
    renderer_test.cpp
    document_test.cpp
    fake_renderer.h
    utils_test.cpp
    element_test.cpp
    table_element_test.cpp image_element_test.cpp code_element_test.cpp)

target_link_libraries(cmark_renderer_test gtest_main cmark_renderer libcmark_static libcmarkextensions_static)
target_include_directories(cmark_renderer_test PRIVATE ${cmark_renderer_SOURCE_DIR}/src)

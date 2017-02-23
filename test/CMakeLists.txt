add_executable(cmark_renderer_test 
    renderer_test.cpp
    document_test.cpp
    utils_test.cpp
    element_test.cpp)

target_link_libraries(cmark_renderer_test gtest_main cmark_renderer)
target_include_directories(cmark_renderer_test PRIVATE ${cmark_renderer_SOURCE_DIR}/src)

cmake_minimum_required (VERSION 3.5)

project (cmark_renderer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_subdirectory(thirdparty)
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(skia_cmark)

cmake_minimum_required (VERSION 3.5)

project (cmark_renderer LANGUAGES CXX)

option(WITH_COVERAGE "Generate code coverage report." OFF)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_subdirectory(thirdparty)

enable_testing()

add_subdirectory(src)
add_subdirectory(test)

if (NOT WITH_COVERAGE)
    add_subdirectory(skia_cmark)
endif()

if(NOT Win32 AND WITH_COVERAGE)
    include(cmake/CodeCoverage.cmake)
    setup_target_for_coverage(cmark_renderer_coverage
        cmark_renderer_test coverage)
endif()
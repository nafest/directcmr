project(skia_cmark LANGUAGES CXX)

# since skia and googletest use the static runtime on default, use
# the static runtime as well for cmark_renderer
if(WIN32)
    string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
    string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
endif()

add_executable(skia_cmark
    main.cpp
    skia_backend.cpp
    skia_backend.h)

target_include_directories(skia_cmark PRIVATE
    ${cmark_renderer_SOURCE_DIR}/src)

if(APPLE)
    target_compile_definitions(skia_cmark PUBLIC SK_BUILD_FOR_MAC)

    target_link_libraries(skia_cmark "-framework CoreFoundation")
    target_link_libraries(skia_cmark "-framework CoreGraphics")
    target_link_libraries(skia_cmark "-framework CoreText")
    target_link_libraries(skia_cmark "-framework Foundation")
    target_link_libraries(skia_cmark "-framework QuartzCore")
    target_link_libraries(skia_cmark "-framework ImageIO")
    target_link_libraries(skia_cmark "-framework AppKit")
    target_link_libraries(skia_cmark "-framework OpenGL")
endif()
if(WIN32)
    target_link_libraries(skia_cmark opengl32.lib)
endif()

target_link_libraries(skia_cmark
    directcmr skia)

if (WITH_COVERAGE)
    set_property(TARGET skia_cmark
        PROPERTY LINK_FLAGS "-fprofile-arcs -ftest-coverage")
endif()


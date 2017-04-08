add_executable(skia_cmark
   main.cpp
   skia_renderer.cpp
   skia_renderer.h)

#set(SKIA_PATH /Users/stefan/Projects/3rdparty/skia/skia_current/skia)
#set(SKIA_OUT /out/Static)

target_include_directories(skia_cmark PRIVATE
    ${cmark_renderer_SOURCE_DIR}/src
    ${SKIA_PATH}/include/core
    ${SKIA_PATH}/include/ports
    ${SKIA_PATH}/include/config
    ${SKIA_PATH}/include/images)

target_compile_definitions(skia_cmark PUBLIC SK_BUILD_FOR_MAC)

target_link_libraries(skia_cmark "-framework CoreFoundation")
target_link_libraries(skia_cmark "-framework CoreGraphics")
target_link_libraries(skia_cmark "-framework CoreText")
target_link_libraries(skia_cmark "-framework Foundation")
target_link_libraries(skia_cmark "-framework QuartzCore")
target_link_libraries(skia_cmark "-framework ImageIO")
target_link_libraries(skia_cmark "-framework AppKit")
target_link_libraries(skia_cmark "-framework OpenGL")

target_link_libraries(skia_cmark
    cmark_renderer
    ${SKIA_PATH}${SKIA_OUT}/libskia.a)

add_dependencies(skia_cmark libskia)


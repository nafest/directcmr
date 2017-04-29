include(ExternalProject)

add_subdirectory (googletest)

if (NOT WITH_COVERAGE)
    ExternalProject_Add(depot_tools
        GIT_REPOSITORY https://chromium.googlesource.com/chromium/tools/depot_tools.git
        UPDATE_DISCONNECTED 1
        BUILD_COMMAND ""
        CONFIGURE_COMMAND ""
        INSTALL_COMMAND ""
        )

    ExternalProject_Get_Property(depot_tools SOURCE_DIR)
    set(DEPOT_TOOLS_DIR ${SOURCE_DIR})

    if(WIN32)
        # due to the messed up way cmake escapes the arguments to gn, the
        # only way I got this to work was writing the comands to a .bat
        # file and call this .bat file as BUILD_COMMAND
        if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
            set(GN_ARGS "extra_cflags_cc=[\\\"/MTd\\\"] extra_cflags_c=[\\\"/MTd\\\"]")
        else()
            set(GN_ARGS "is_official_build=true")
        endif()
            set(SKIA_BUILD_BAT ${CMAKE_CURRENT_BINARY_DIR}/build_skia.bat)
            file(WRITE ${SKIA_BUILD_BAT} "python tools/git-sync-deps\n")
            file(APPEND ${SKIA_BUILD_BAT} "bin\\gn gen out/Static --args=\"${GN_ARGS}\"\n")
            file(APPEND ${SKIA_BUILD_BAT} "ninja -C out/Static skia\n")
        set(SKIA_BUILD_COMMAND
            COMMAND
            COMMAND ${CMAKE_COMMAND} -E chdir <SOURCE_DIR> ${SKIA_BUILD_BAT})
    else()
        set(SKIA_BUILD_COMMAND COMMAND cd <SOURCE_DIR> && export PATH=${DEPOT_TOOLS_DIR}:$ENV{PATH} && python tools/git-sync-deps && bin/gn gen out/Static --args=is_official_build=true && ninja -C out/Static skia)
    endif()

    ExternalProject_Add(libskia
        GIT_REPOSITORY https://skia.googlesource.com/skia.git
        UPDATE_DISCONNECTED 1
        GIT_TAG chrome/m58
        DEPENDS depot_tools
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ${SKIA_BUILD_COMMAND}
        BUILD_BYPRODUCTS
            <SOURCE_DIR>/out/Static/${CMAKE_STATIC_LIBRARY_PREFIX}skia${CMAKE_STATIC_LIBRARY_SUFFIX}
            <SOURCE_DIR>/include/core
            <SOURCE_DIR>/include/ports
            <SOURCE_DIR>/include/config
        INSTALL_COMMAND "")

    ExternalProject_Get_Property(libskia SOURCE_DIR)

    # INTERFACE_INCLUDE_DIRECTORIES does not accept non-existent paths
    # (https://gitlab.kitware.com/cmake/cmake/issues/15052)
    # As a workaround create the include directories on configuration
    file(MAKE_DIRECTORY ${SOURCE_DIR}/include/core)
    file(MAKE_DIRECTORY ${SOURCE_DIR}/include/ports)
    file(MAKE_DIRECTORY ${SOURCE_DIR}/include/config)

    add_library(skia STATIC IMPORTED GLOBAL)
    set_target_properties(skia PROPERTIES IMPORTED_LOCATION ${SOURCE_DIR}/out/Static/${CMAKE_STATIC_LIBRARY_PREFIX}skia${CMAKE_STATIC_LIBRARY_SUFFIX})
    set_property(TARGET skia PROPERTY INTERFACE_INCLUDE_DIRECTORIES
        ${SOURCE_DIR}/include/core
        ${SOURCE_DIR}/include/ports
        ${SOURCE_DIR}/include/config)
    add_dependencies(skia libskia)
endif()

# since skia and googletest use the static runtime on default, use
# the static runtime as well for cmark
if(WIN32)
    string(REPLACE "/MD" "/MT" CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
    string(REPLACE "/MD" "/MT" CMAKE_C_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
    string(REPLACE "/MD" "/MT" CMAKE_C_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE})
    string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
    string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
endif()

add_subdirectory (cmark)

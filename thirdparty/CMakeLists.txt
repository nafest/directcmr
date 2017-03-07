include(ExternalProject)

add_subdirectory (googletest)
add_subdirectory (cmark)

ExternalProject_Add(depot_tools
    GIT_REPOSITORY https://chromium.googlesource.com/chromium/tools/depot_tools.git
    UPDATE_DISCONNECTED 1
    BUILD_COMMAND ""
    CONFIGURE_COMMAND ""
    INSTALL_COMMAND ""
    )

ExternalProject_Get_Property(depot_tools SOURCE_DIR)
set(DEPOT_TOOLS_DIR ${SOURCE_DIR})

ExternalProject_Add(libskia
    GIT_REPOSITORY https://skia.googlesource.com/skia.git
    UPDATE_DISCONNECTED 1
    GIT_TAG chrome/m58
    DEPENDS depot_tools
    #CONFIGURE_COMMAND cd <SOURCE_DIR> && export PATH=${DEPOT_TOOLS_DIR}:$ENV{PATH} && python tools/git-sync-deps && bin/gn gen out/Static --args=is_official_build=true
    #BUILD_COMMAND cd <SOURCE_DIR> && export PATH=${DEPOT_TOOLS_DIR}:$ENV{PATH} && ninja -C out/Static
    CONFIGURE_COMMAND ""
    BUILD_COMMAND cd <SOURCE_DIR> && export PATH=${DEPOT_TOOLS_DIR}:$ENV{PATH} && python tools/git-sync-deps && bin/gn gen out/Static --args=is_official_build=true && ninja -C out/Static
    INSTALL_COMMAND "")

ExternalProject_Get_Property(libskia SOURCE_DIR)
# SKIA_PATH and SKIA_OUT are used in skia_cmark
set(SKIA_PATH ${SOURCE_DIR} PARENT_SCOPE)
set(SKIA_OUT /out/Static PARENT_SCOPE)

project(directcmr_test LANGUAGES CXX)

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

add_executable(directcmr_test
    backend_test.cpp
    document_test.cpp
    fake_backend.h
    utils_test.cpp
    element_test.cpp
    table_element_test.cpp image_element_test.cpp code_element_test.cpp thematic_break_element_test.cpp link_element_test.cpp linebreak_element_test.cpp utf8_test.cpp)

target_link_libraries(directcmr_test gtest_main directcmr libcmark_static libcmarkextensions_static)
target_include_directories(directcmr_test PRIVATE ${directcmr_SOURCE_DIR}/src)

if (WITH_COVERAGE)
    # since a lot of code from headers is instanciated in the
    # test code, set the coverage flags for the test code as well
    set_property(TARGET directcmr_test
        PROPERTY COMPILE_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
    set_property(TARGET directcmr_test
        PROPERTY LINK_FLAGS "-fprofile-arcs -ftest-coverage")
endif()

add_test(directcmr_test directcmr_test)
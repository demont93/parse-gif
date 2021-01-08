cmake_minimum_required(VERSION 3.19)
project(parsegif)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)
set_target_properties(parsegif PROPERTIES PUBLIC_HEADER include/parsegif.h)

if (CMAKE_TESTING_ENABLED)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup(TARGETS)
    add_subdirectory(tests)
    add_test (NAME tests COMMAND Test)
    add_test (NAME test_decoder COMMAND Test)
endif()

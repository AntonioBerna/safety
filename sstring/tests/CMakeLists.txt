cmake_minimum_required(VERSION 3.10)
project(sstring_tests)

set(CMAKE_C_STANDARD 99)

include_directories(../core)

add_library(sstring STATIC ../core/sstring.c)
add_executable(sstring-test sstring-test.c)
target_link_libraries(sstring-test sstring)
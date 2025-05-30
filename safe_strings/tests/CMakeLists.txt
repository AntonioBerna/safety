cmake_minimum_required(VERSION 3.10)
project(safe_strings_tests)

set(CMAKE_C_STANDARD 99)

include_directories(../core)

add_library(safe_strings STATIC ../core/safe_strings.c)
add_executable(safe_strings_test safe_strings_test.c)
target_link_libraries(safe_strings_test safe_strings)
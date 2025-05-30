cmake_minimum_required(VERSION 3.10)
project(full_example)

set(CMAKE_C_STANDARD 99)

include_directories(../core)

add_library(safe_strings STATIC ../core/safe_strings.c)
add_executable(full_example full_example.c)
target_link_libraries(full_example safe_strings)
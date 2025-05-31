cmake_minimum_required(VERSION 3.10)
project(full_example)

set(CMAKE_C_STANDARD 99)

include_directories(../core)

add_library(sstring STATIC ../core/sstring.c)
add_executable(full-example full-example.c)
target_link_libraries(full-example sstring)
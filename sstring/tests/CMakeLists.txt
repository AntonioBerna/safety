cmake_minimum_required(VERSION 3.10)
project(sstring_tests)

# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Use Clang as the compiler
set(CMAKE_C_COMPILER clang)

# Set strict compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -pedantic")

include_directories(../core)

add_library(sstring STATIC ../core/sstring.c)
add_executable(sstring-test sstring-test.c)
target_link_libraries(sstring-test sstring)
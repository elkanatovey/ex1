cmake_minimum_required(VERSION 3.12)
project(ex1 C)

set(CMAKE_C_STANDARD 99)

add_executable(ex1 AnalyzeProtein.c)
target_link_libraries(ex1 m)
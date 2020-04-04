cmake_minimum_required(VERSION 3.8)

project(superstring)

set(CMAKE_CXX_STANDARD          17)

add_subdirectory(contrib/googletest)

add_library(graph_lib STATIC    graph.cpp assigment.cpp)
add_executable(run              run.cpp)
add_executable(test_superstring test.cpp)

target_link_libraries(test_superstring graph_lib gtest gtest_main pthread)
target_link_libraries(run graph_lib graph_lib)
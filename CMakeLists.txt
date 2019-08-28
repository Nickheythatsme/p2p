cmake_minimum_required(VERSION 3.9)
set(CMAKE_CXX_STANDARD 17)
project(p2p)

enable_testing()

add_subdirectory(src)
add_subdirectory(tests)

# TODO remove scratch exec
add_executable(p2p main.cpp)
target_link_libraries(p2p p2putil)


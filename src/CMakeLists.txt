# TODO set up libraries

find_package(Threads)

add_executable(p2p main.cpp)

target_link_libraries (p2p ${CMAKE_THREAD_LIBS_INIT})


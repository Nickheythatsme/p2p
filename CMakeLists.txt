# TODO add more documentation here
cmake_minimum_required (VERSION 3.9)
set (CMAKE_CXX_STANDARD 17)
project (p2p)

find_package(Boost REQUIRED)
IF (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif()

enable_testing()

# Recurse into the "Hello" and "Demo" subdirectories. This does not actually
# cause another cmake executable to run. The same process will walk through
# the project's entire directory structure.
add_subdirectory (src)
add_subdirectory (tests)

# TODO add more documentation here
cmake_minimum_required (VERSION 3.9)
set (CMAKE_CXX_STANDARD 11)
project (p2p)


# Recurse into the "Hello" and "Demo" subdirectories. This does not actually
# cause another cmake executable to run. The same process will walk through
# the project's entire directory structure.
add_subdirectory (src)
add_subdirectory (tests)

enable_testing()

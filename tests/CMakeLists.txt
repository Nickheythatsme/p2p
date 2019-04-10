cmake_minimum_required (VERSION 3.9)

configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# Find Boost
find_package(Boost 1.69 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# Making tests here
add_executable(example_test example_test.cpp)
target_link_libraries(example_test gtest_main)
add_test(NAME example_test COMMAND example_test)

add_executable(node_test node_test.cpp)
target_link_libraries(node_test gtest_main Boost::boost)
add_test(NAME node_test COMMAND node_test)



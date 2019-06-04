cmake_minimum_required (VERSION 3.9)

### Set up googletest
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
### END Set up googletest

### Add/Make tests
add_executable(example_test example_test.cpp ../src/util/config.hpp)
target_link_libraries(example_test gtest_main)
add_test(NAME example_test COMMAND example_test)

add_executable(node_test node_test.cpp)
target_link_libraries(node_test gtest_main)
add_test(NAME node_test COMMAND node_test)

add_executable(logger_test logger_test.cpp)
target_link_libraries(logger_test gtest_main)
add_test(NAME logger_test COMMAND logger_test)

add_executable(listener_test listener_test.cpp)
target_link_libraries(listener_test gtest_main)
add_test(NAME listener_test COMMAND listener_test)

add_executable(ping_test ping_test.cpp)
target_link_libraries(ping_test gtest_main)
add_test(NAME ping_test COMMAND ping_test)

add_executable(uuid_test uuid_test.cpp ../src/uuid.cpp)
target_link_libraries(uuid_test gtest_main)
add_test(NAME uuid_test COMMAND uuid_test)

### END Add/Make tests

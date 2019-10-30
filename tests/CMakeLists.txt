cmake_minimum_required(VERSION 3.9)

### Set up googletest
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if (result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif ()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if (result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif ()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)
### END Set up googletest

### Find packages
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Include source
include_directories(${PROJECT_SOURCE_DIR}/src)

### Add/Make tests
add_executable(example_test example_test.cpp)
target_link_libraries(example_test gtest_main)
add_test(NAME example_test COMMAND example_test)

add_executable(hash_test hash_test.cpp)
target_link_libraries(hash_test gtest_main p2pcrypto)
add_test(NAME hash_test COMMAND hash_test)

add_executable(uuid_test uuid_test.cpp)
target_link_libraries(uuid_test gtest_main p2putil)
add_test(NAME uuid_test COMMAND uuid_test)

add_executable(hash_table_test hash_table_test.cpp)
target_link_libraries(hash_table_test gtest_main p2pcrypto)
add_test(NAME hash_table_test COMMAND hash_table_test)

add_executable(record_test record_test.cpp)
target_link_libraries(record_test gtest_main p2pmain p2putil)
add_test(NAME record_test COMMAND record_test)

add_executable(listener_test listener_test.cpp)
target_link_libraries(listener_test gtest_main p2pmain p2putil p2pnetworking)
add_test(NAME listener_test COMMAND listener_test)

add_executable(serialize_test serialize_test.cpp)
target_link_libraries(serialize_test gtest_main p2pnetworking)
add_test(NAME serialize_test COMMAND serialize_test)

add_executable(key_test key_test.cpp)
target_link_libraries(key_test gtest_main p2putil)
add_test(NAME key_test COMMAND key_test)
### END Add/Make tests

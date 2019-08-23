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

### Add/Make tests
add_executable(example_test example_test.cpp)
target_link_libraries(example_test gtest_main)
add_test(NAME example_test COMMAND example_test)

add_executable(hash_test
        hash_test.cpp
        ../src/crypto/sha256.cpp
        ../src/crypto/sha256.h
        ../src/crypto/endian.h
        ../src/crypto/byteswap.h
        ../src/crypto/common.h
        )
target_link_libraries(hash_test gtest_main)
add_test(NAME hash_test COMMAND hash_test)

add_executable(uuid_test uuid_test.cpp ../src/uuid.cpp ../src/file_table.cpp ../src/file_table.h)
target_link_libraries(uuid_test gtest_main)
add_test(NAME uuid_test COMMAND uuid_test)

add_executable(hash_table_test
        hash_table_test.cpp
        ../src/crypto/sha256.cpp
        ../src/crypto/sha256.h
        ../src/crypto/endian.h
        ../src/crypto/byteswap.h
        ../src/crypto/common.h
        )
target_link_libraries(hash_table_test gtest_main)
add_test(NAME hash_table_test COMMAND hash_table_test)

add_executable(file_table_test
        file_table_test.cpp
        ../src/file_table.cpp
        ../src/crypto/sha256.cpp
        ../src/crypto/sha256.h
        ../src/crypto/endian.h
        ../src/crypto/byteswap.h
        ../src/crypto/common.h
        )
target_link_libraries(file_table_test gtest_main)
add_test(NAME file_table_test COMMAND file_table_test)
### END Add/Make tests

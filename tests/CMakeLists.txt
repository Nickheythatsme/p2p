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


### Find Boost
#set(Boost_USE_STATIC_LIBS ON) 
#set(Boost_USE_MULTITHREADED ON)  
#set(Boost_USE_STATIC_RUNTIME OFF) 
#set(Boost_NO_SYSTEM_PATHS TRUE)
#
#if ((Boost_NO_SYSTEM_PATHS) AND (NOT BOOST_ROOT))
#    message (FATAL_ERROR "BOOST_ROOT is not set. Stopping.")
#else()
#    set(BOOST_INCLUDEDIR "${BOOST_ROOT}/boost")
#    set(BOOST_LIBRARYDIR "${BOOST_ROOT}/stage/lib")
#    message (STATUS "BOOST_ROOT: ${BOOST_ROOT}")
#    message (STATUS "BOOST_INCLUDEDIR: ${BOOST_INCLUDEDIR}")
#    message (STATUS "BOOST_LIBRARYDIR: ${BOOST_LIBRARYDIR}")
#    find_package(Boost 1.69 REQUIRED COMPONENTS iostreams thread chrono system)
#    if (Boost_FOUND)
#        include_directories(${Boost_INCLUDE_DIRS})
#        message (STATUS "Found the following Boost libraries: " ${Boost_LIBRARIES})
#    else()
#        message (SEND_ERROR "Boost not found. See logs above")
#    endif()
#endif()
### END Find Boost


### Add/Make tests
add_executable(example_test example_test.cpp)
target_link_libraries(example_test gtest_main)
add_test(NAME example_test COMMAND example_test)

add_executable(node_test node_test.cpp)
target_link_libraries(node_test gtest_main)
add_test(NAME node_test COMMAND node_test)

add_executable(server_test server_test.cpp)
target_link_libraries(server_test gtest_main)
add_test(NAME server_test COMMAND server_test)
### END Add/Make tests


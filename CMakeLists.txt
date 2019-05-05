cmake_minimum_required(VERSION 3.9)
set (CMAKE_CXX_STANDARD 17)
project (p2p)

### Set up Boost
#set( BOOST_COMPONENTS_FOR_BUILD "--with-type_erasure --with-iostreams --with-thread")
#set( BOOST_INSTALL_ROOT ${CMAKE_CURRENT_BINARY_DIR}/boost )
#set( BOOST_ROOT ${BOOST_INSTALL_ROOT}/boost-prefix/src/boost )
#
#if (NOT CPU_COUNT)
#    set(CPU_COUNT 1)
#endif()
#message(STATUS "Building with ${CPU_COUNT} cpu(s)")
#message(STATUS "Building boost components ${BOOST_COMPONENTS_FOR_BUILD}")
#
#configure_file(CMakeLists.txt.in boost/CMakeLists.txt)
#execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
#    RESULT_VARIABLE result
#    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/boost )
#if(result)
#    message(FATAL_ERROR "CMake step for boost failed: ${result}")
#endif()
#execute_process(COMMAND ${CMAKE_COMMAND} --build .
#    RESULT_VARIABLE result
#    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/boost )
#if(result)
#    message(FATAL_ERROR "Build step for boost failed: ${result}")
#endif()
#add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/boost
#                 EXCLUDE_FROM_ALL)
### Finish Set up Boost

enable_testing()

add_subdirectory (src)
add_subdirectory (tests)

## Build the banded_alignment application and its test suite.
cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Set the project name
project (banded_alignment)
# check if trying to build within project
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file).
    Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()
## BOOST LIBRARY
#set(Boost_USE_MULTITHREADED      ON)
#set(BOOST_MINIMUM_VERSION 1.69.0)
#set(BOOST_REQUIRED_LIBRARIES system date_time filesystem context iostreams coroutine)
#include(cmake/addBoost.cmake)
# LIBGABA LIBRARY
add_subdirectory(extern)
# create library
add_library(ba STATIC src/BandedAlignment.cpp)
target_link_libraries(ba libgaba)
target_include_directories(ba
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include)
# Add an executable
add_executable(bandedAlignment src/main.cpp)
target_link_libraries(bandedAlignment PRIVATE ba)
# Pybind LIBRARY
find_program(PYTHON_EXECUTABLE NAMES python HINTS $ENV{PATH})
include(cmake/AddPybind.cmake RESULT_VARIABLE pybind)
# add pybind
pybind11_add_module(bindings src/PythonBindings.cpp)
target_link_libraries(bindings PRIVATE ba)
# add tests
add_subdirectory(tests)

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
message(Python_ROOT_DIR: "${Python_ROOT_DIR}")
message(PYTHON_EXECUTABLE: "${PYTHON_EXECUTABLE}")
message(PYTHON_LIBRARY: "${PYTHON_LIBRARY}")

#message("$ENV{PATH}")
find_program(PYTHON_EXECUTABLE NAMES python NO_PACKAGE_ROOT_PATH NO_CMAKE_PATH HINTS $ENV{PATH})
message("${PYTHON_EXECUTABLE}")
#message("${Python3_FIND_VIRTUALENV}")
set(Python3_FIND_VIRTUALENV "FIRST")
#set(Python_ROOT_DIR "/Users/andrewbailey/anaconda3/envs/ba/")
#set(PYTHON_EXECUTABLE "/Users/andrewbailey/anaconda3/envs/ba/python3.7")
#set(PYTHON_LIBRARY "/Users/andrewbailey/anaconda3/envs/ba/lib/libpython3.7m.dylib")
#message(PYTHON_EXECUTABLE: "${PYTHON_EXECUTABLE}")
#message(PYTHON_LIBRARY: "${PYTHON_LIBRARY}")
#message(Python_ROOT_DIR: "${Python_ROOT_DIR}")
#set(Python3_DIR  "${Python_ROOT_DIR}")
#message(Python3_DIR: "${Python3_DIR}")
#find_package(Python3 COMPONENTS Interpreter Development)
#find_package(Python3 3.7 COMPONENTS Interpreter Development NO_PACKAGE_ROOT_PATH NO_CMAKE_PATH HINTS ${Python3_ROOT_DIR})
#find_package (Python3 COMPONENTS Interpreter Development NO_PACKAGE_ROOT_PATH NO_CMAKE_PATH  HINTS $ENV{PATH} )
message(PYTHON_EXECUTABLE: "${PYTHON_EXECUTABLE}")
message(PYBIND11_PYTHON_VERSION: "${PYBIND11_PYTHON_VERSION}")
set(Python_ADDITIONAL_VERSIONS 3.7 3.6 3.5 3.4)
if(PYTHONLIBS_FOUND AND PYTHON_MODULE_EXTENSION)
    return()
endif()
find_library(PYTHON_LIBRARY
        NAMES "python${PYTHON_LIBRARY_SUFFIX}"
        PATHS ${_PYTHON_LIBS_SEARCH}
        NO_DEFAULT_PATH)

# Use the Python interpreter to find the libs.
message(PythonLibsNew_FIND_REQUIRED "${PythonLibsNew_FIND_REQUIRED}")
message(PythonLibsNew_FIND_VERSION "${PythonLibsNew_FIND_VERSION}")

if(PythonLibsNew_FIND_REQUIRED)
    find_package(PythonInterp ${PythonLibsNew_FIND_VERSION} REQUIRED)
else()
    find_package(PythonInterp ${PythonLibsNew_FIND_VERSION})
endif()
include(cmake/addPybind.cmake RESULT_VARIABLE pybind)
message(PYTHON_EXECUTABLE: "${PYTHON_EXECUTABLE}")
message(PYTHON_LIBRARY: "${PYTHON_LIBRARY}")
# add pybind
pybind11_add_module(bindings src/PythonBindings.cpp)
target_link_libraries(bindings PRIVATE ba)
# add tests
add_subdirectory(tests)

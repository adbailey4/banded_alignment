## Build test suite.
project(test_banded_alignment)
enable_testing()

include(../cmake/AddGoogleTest.cmake)

add_executable(CppTests src/AllTests.cpp)
target_link_libraries(CppTests PRIVATE ba)
add_gtest(CppTests)

add_test(NAME test_bindings COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/python_tests/test_bindings.py
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})


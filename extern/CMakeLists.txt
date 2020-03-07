## Build test suite.
project(libgaba)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(LIBGABA_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/libgaba/libgaba.a)
set(LIBGABA_HEADER_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/libgaba)

set(ENV{CFLAGS} "-fPIC -O3 -Wall -Wno-unused-function -Wno-unused-label -std=c99 -pipe -DBIT=4")

add_custom_command(
        OUTPUT ${LIBGABA_LIBRARY}
        COMMAND make
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libgaba)
unset(ENV{CFLAGS})
add_custom_target(libgaba_target DEPENDS ${LIBGABA_LIBRARY})
add_library(${PROJECT_NAME} STATIC IMPORTED GLOBAL)
add_dependencies(${PROJECT_NAME} libgaba_target)

set_target_properties(${PROJECT_NAME}
        PROPERTIES
        IMPORTED_LOCATION ${LIBGABA_LIBRARY}
        INTERFACE_INCLUDE_DIRECTORIES ${LIBGABA_HEADER_FOLDER})

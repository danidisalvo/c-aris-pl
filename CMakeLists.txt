cmake_minimum_required(VERSION 3.28)
project(aris_pl C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -g  -fno-omit-frame-pointer -O1")

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB REQUIRED glib-2.0)

include_directories(${GLIB_INCLUDE_DIRS})
link_directories(${GLIB_LIBRARY_DIRS})
add_definitions(${GLIB_CFLAGS_OTHER})

add_executable(aris_pl
        src/main.c
        src/lexical_analyzer.c
        src/token.c
        src/parser.c
        src/wff.c
        include/lexical_analyzer.h
        include/parser.h
        include/token.h
        include/wff.h
)

target_link_libraries(aris_pl ${GLIB_LIBRARIES})

add_subdirectory(test)

add_custom_target(run_all_tests
        COMMAND test/test_lexical_analyzer
        COMMAND test/test_parser
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

#/Applications/CLion.app/Contents/bin/cmake/mac/aarch64/bin/cmake

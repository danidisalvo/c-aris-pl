add_executable(test_aris
        test.c
        parser/test_parser.c
        lexical_analyzer/test_lexical_analyzer.c
        parser/test_parser.c
        ../src/lexical_analyzer.c
        ../src/parser.c
        ../src/token.c
        ../src/wff.c
        lexical_analyzer/test_lexical_analizer.h
        parser/test_parser.h
)

target_include_directories(test_aris PRIVATE
        ${GLIB_INCLUDE_DIRS}
        ../include
)

target_link_libraries(test_aris ${GLIB_LIBRARIES})

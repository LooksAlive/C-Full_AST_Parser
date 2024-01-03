cmake_minimum_required(VERSION 3.5)

project(C_P_I LANGUAGES C)

set(CMAKE_C_COMPILER "/usr/bin/gcc")

set(CMAKE_C_FLAGS "-g -O0")


set (SOURCES
    main.h
    ast_dumper.h
    generic_vector.h
    lexer.h
    lexer.c
    )


add_executable(C_P_I main.c ${SOURCES})

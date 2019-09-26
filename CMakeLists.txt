cmake_minimum_required(VERSION 3.15)
project(pielang C)

set(CMAKE_C_STANDARD 99)

add_executable(pielang main.c linenoise.h linenoise.c bool.h hashtable.h hashtable.c lexer.h lexer.c ast.h ast.c value.h value.c scope.h scope.c evaluator.h evaluator.c utils.h utils.c system.c system.h)

target_link_libraries(pielang m)

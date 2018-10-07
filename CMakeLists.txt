cmake_minimum_required(VERSION 3.12)
project(pilang)

set(CMAKE_CXX_STANDARD 14)

add_executable(pilang main.cpp scope.h variable.h userfunction.h utils.h stringutils.h regexes.h options.h stringliteral.h userfunction.cpp options.cpp regexes.cpp scope.cpp stringliteral.cpp stringutils.cpp utils.cpp variable.cpp logger.cpp logger.h termcolor.hpp value.cpp value.h expression.cpp expression.h operation.cpp operation.h systemfunction.cpp systemfunction.h system.cpp system.h function.cpp function.h object.cpp object.h)
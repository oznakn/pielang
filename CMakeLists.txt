cmake_minimum_required(VERSION 3.12)
project(PieLang)

set(CMAKE_CXX_STANDARD 17)

add_executable(PieLang main.cpp scope.h variable.h userfunction.h utils.h stringutils.h options.h stringliteral.h userfunction.cpp options.cpp scope.cpp stringliteral.cpp stringutils.cpp utils.cpp variable.cpp logger.cpp logger.h termcolor.hpp value.cpp value.h expression.cpp expression.h operation.cpp operation.h systemfunction.cpp systemfunction.h system.cpp system.h function.cpp function.h object.cpp object.h definitions.h shell.cpp shell.h operator.cpp operator.h)
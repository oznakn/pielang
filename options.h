#ifndef PILANG_OPTIONS_H
#define PILANG_OPTIONS_H

#include <string>

using namespace std;

struct Options {
    const static string MULTI_COMMENT_START_STRING;
    const static string MULTI_COMMENT_END_STRING;
    const static char STRING_CHAR;
    const static char STRING_ESCAPE_CHAR;
    const static char STRING_LITERAL_CHAR;
    const static char END_OF_LINE_OPTIONAL_CHAR;
    const static string END_OF_LINE_OPTIONAL_CHAR_AS_STRING;
    const static string END_OF_LINE;
    const static string END_OF_LINE_WITH_OPTIONAL_CHAR;
    const static string WINDOWS_END_OF_LINE;
    const static string WINDOWS_END_OF_LINE_WITH_OPTIONAL_CHAR;
    const static string FUNCTION_WORD_STRING;
    const static char START_PARENTHESIS_CHAR;
    const static char END_PARENTHESIS_CHAR;
    const static char START_BLOCK_CHAR;
    const static char END_BLOCK_CHAR;
};

#endif //PILANG_OPTIONS_H

#ifndef PILANG_OPTIONS_H
#define PILANG_OPTIONS_H

#include <string>

struct Options {
    const static std::string MULTI_COMMENT_START_STRING;
    const static std::string MULTI_COMMENT_END_STRING;
    const static char EQUALITY_CHAR;
    const static std::string CHECK_EQUALITY_STRING;
    const static char NUMBER_SEP_CHAR;
    const static char STRING_CHAR;
    const static char STRING_ESCAPE_CHAR;
    const static char STRING_LITERAL_CHAR;
    const static char END_OF_LINE_OPTIONAL_CHAR;
    const static std::string END_OF_LINE_OPTIONAL_CHAR_AS_STRING;
    const static std::string END_OF_LINE;
    const static std::string END_OF_LINE_WITH_OPTIONAL_CHAR;
    const static std::string WINDOWS_END_OF_LINE;
    const static std::string WINDOWS_END_OF_LINE_WITH_OPTIONAL_CHAR;
    const static std::string FUNCTION_WORD_STRING;
    const static char START_PARENTHESIS_CHAR;
    const static char END_PARENTHESIS_CHAR;
    const static char START_BLOCK_CHAR;
    const static char END_BLOCK_CHAR;
    const static std::string BOOL_TRUE_STRING;
    const static std::string BOOL_FALSE_STRING;
};

#endif //PILANG_OPTIONS_H

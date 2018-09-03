#include "options.h"

const string Options::MULTI_COMMENT_START_STRING = "/*";
const string Options::MULTI_COMMENT_END_STRING = "*/";

const char Options::STRING_CHAR = '"';
const char Options::STRING_ESCAPE_CHAR = '\\';
const char Options::STRING_LITERAL_CHAR = '~';

const char Options::END_OF_LINE_OPTIONAL_CHAR = ';';
const string Options::END_OF_LINE_OPTIONAL_CHAR_AS_STRING = string(1, Options::END_OF_LINE_OPTIONAL_CHAR);

const string Options::END_OF_LINE = "\n";
const string Options::WINDOWS_END_OF_LINE = "\n\r";
const string Options::END_OF_LINE_WITH_OPTIONAL_CHAR = Options::END_OF_LINE_OPTIONAL_CHAR_AS_STRING + Options::END_OF_LINE;
const string Options::WINDOWS_END_OF_LINE_WITH_OPTIONAL_CHAR = Options::END_OF_LINE_OPTIONAL_CHAR_AS_STRING + Options::WINDOWS_END_OF_LINE;

const string Options::FUNCTION_WORD_STRING = "function";

const char Options::START_PARENTHESIS_CHAR = '(';
const char Options::END_PARENTHESIS_CHAR = ')';
const char Options::START_BLOCK_CHAR = '{';
const char Options::END_BLOCK_CHAR = '}';

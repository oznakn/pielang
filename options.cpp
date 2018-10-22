#include "options.h"

const std::string Options::COMMENT_START_STRING = "//";
const std::string Options::MULTI_COMMENT_START_STRING = "/*";
const std::string Options::MULTI_COMMENT_END_STRING = "*/";

const char Options::EQUALITY_CHAR = '=';
const std::string Options::CHECK_EQUALITY_STRING = std::string(1, Options::EQUALITY_CHAR) + Options::EQUALITY_CHAR;

const char Options::DOT_CHAR = '.';

const char Options::STRING_CHAR = '"';
const char Options::STRING_ESCAPE_CHAR = '\\';
const char Options::STRING_LITERAL_CHAR = '~';

const char Options::END_OF_LINE_OPTIONAL_CHAR = ';';
const std::string Options::END_OF_LINE_OPTIONAL_CHAR_AS_STRING = std::string(1, Options::END_OF_LINE_OPTIONAL_CHAR);

const std::string Options::END_OF_LINE = "\n";
const std::string Options::WINDOWS_END_OF_LINE = "\n\r";
const std::string Options::CLASSIC_MAC_END_OF_LINE = "\r";
const std::string Options::END_OF_LINE_WITH_OPTIONAL_CHAR = Options::END_OF_LINE_OPTIONAL_CHAR_AS_STRING + Options::END_OF_LINE;
const std::string Options::WINDOWS_END_OF_LINE_WITH_OPTIONAL_CHAR = Options::END_OF_LINE_OPTIONAL_CHAR_AS_STRING + Options::WINDOWS_END_OF_LINE;
const std::string Options::CLASSIC_MAC_END_OF_LINE_WITH_OPTIONAL_CHAR = Options::END_OF_LINE_OPTIONAL_CHAR_AS_STRING + Options::CLASSIC_MAC_END_OF_LINE;

const std::string Options::FUNCTION_WORD_STRING = "def";

const char Options::START_PARENTHESIS_CHAR = '(';
const char Options::END_PARENTHESIS_CHAR = ')';
const char Options::START_BLOCK_CHAR = '{';
const char Options::END_BLOCK_CHAR = '}';

const std::string Options::START_PARENTHESIS_STRING = std::string(1, Options::START_PARENTHESIS_CHAR);
const std::string Options::END_PARENTHESIS_STRING = std::string(1, Options::END_PARENTHESIS_CHAR);
const std::string Options::START_BLOCK_STRING = std::string(1, Options::START_BLOCK_CHAR);
const std::string Options::END_BLOCK_STRING = std::string(1, Options::END_BLOCK_CHAR);

const std::string Options::BOOL_TRUE_STRING = "true";
const std::string Options::BOOL_FALSE_STRING = "false";


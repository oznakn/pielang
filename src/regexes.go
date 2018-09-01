package main

const (
	COMMENT_FIND_REGEX                  = `\/\*.*?\*\/`
	NEWLINE_FIND_REGEX                  = `;?[\n\r]`
	FUNCTION_FIRST_LINE_FIND_REGEX      = `(function .+?\(.*?\).*?\{)`
	STRINGS_FIND_REGEX                  = `("([^"]|"")*")` // `(["'])(?:(?=(\\?))\2.)*?\1`
	IS_VALID_VARIABLE_NAME              = `^[a-zA-Z][a-zA-Z0-9_]+$`
	ADDITION_SUBSTRACTION_SPLIT_REGEX   = `[\+\-]`
	MULTIPLICATION_DIVISION_SPLIT_REGEX = `[\*\/]`
)

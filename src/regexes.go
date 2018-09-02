package main

const (
	COMMENT_FIND_REGEX                 = `((\/\*[\s\S]*?\*\/)|(\/\/.*?($|\n)))`
	NEWLINE_FIND_REGEX                 = `;?[\n\r]`
	FUNCTION_FIRST_LINE_FIND_REGEX     = `(function .+?\(.*?\).*?\{)`
	IF_FIND_REGEX                      = `if\s*\((?:[^)(]+|\((?:[^)(]+|\([^)(]*\))*\))*\)\s*\{(?:[^}{]+|\{(?:[^}{]+|\{[^}{]*\})*\})*\}`
	WHILE_FIND_REGEX                   = `while\s*\((?:[^)(]+|\((?:[^)(]+|\([^)(]*\))*\))*\)\s*\{(?:[^}{]+|\{(?:[^}{]+|\{[^}{]*\})*\})*\}`
	STRINGS_FIND_REGEX                 = `("[^"\\\n\r]*(?:\\.[^"\\\n\r]*)*")` + "|" + "(`[^`" + `\\` + "]*(?:" + `\\` + ".[^`" + `\\` + "]*)*`)"
	IS_VALID_VARIABLE_NAME             = `^[a-zA-Z][a-zA-Z0-9_]+$`
	FIND_OUTER_PARANTHESIS_GROUP_REGEX = `\((?:[^)(]+|\((?:[^)(]+|\([^)(]*\))*\))*\)`
)

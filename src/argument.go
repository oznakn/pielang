package main

type ArgumentType int

const (
	ARGUMENT_TYPE_VARIABLE ArgumentType = 0
	ARGUMENT_TYPE_FUNCTION ArgumentType = 1
)

type Argument struct {
	argumentType ArgumentType
	variable     *Variable
	function     *Function
}

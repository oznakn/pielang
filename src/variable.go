package main

import (
	"regexp"
)

type Variable struct {
	variableName string
	value        *Value
}

func (variable *Variable) String() string {
	return variable.variableName + ": " + variable.value.GetAsString()
}

func CreateVariable(variableName string, value *Value) *Variable {
	return &Variable{variableName, value}
}

func IsValidVariableName(decleration string) bool {
	regex := regexp.MustCompile(IS_VALID_VARIABLE_NAME)

	return regex.MatchString(decleration)
}

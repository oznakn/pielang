package main

import (
	"regexp"
	"strings"
)

type Scope struct {
	isMainScope    bool
	initialContent string
	content        string
	lines          []string
	variableList   map[string]*Variable
	functionList   map[string]*Function
}

func (scope *Scope) SetAsMainScope() {
	scope.isMainScope = true
}

func (scope *Scope) FindVariable(variableName string) *Variable {
	if variable, ok := scope.variableList[variableName]; ok {
		return variable
	}

	panic("Undefined Variable")
}

func (scope *Scope) FindFunction(functionName string) *Function {
	if function, ok := scope.functionList[functionName]; ok {
		return function
	}

	panic("Undefined Function")
}

func (scope *Scope) FindMainFunction(functionName string) *Function {
	return scope.FindFunction("main")
}

func (scope *Scope) Run() {
	var regex *regexp.Regexp
	scope.content = scope.initialContent

	regex = regexp.MustCompile(NEWLINE_FIND_REGEX)
	scope.content = regex.ReplaceAllString(scope.content, `$1;$2`)

	if scope.content[len(scope.content)-1] == ';' {
		scope.content = scope.content[:len(scope.content)-1]
	}

	regex = regexp.MustCompile(FUNCTION_FIND_REGEX)

	if scope.isMainScope {
		scope.content = regex.ReplaceAllStringFunc(scope.content,
			func(value string) string {
				function := ParseFunction(scope, value)

				scope.functionList[function.functionName] = function

				return ""
			})
	} else {
		if len(regex.FindAllString(scope.content, 1)) > 0 {
			panic("You cannot declare a function in a function")
		}

		scope.lines = strings.Split(scope.content, ";")

		for _, line := range scope.lines {
			RunLineRunner(scope, line)
		}
	}
}

func CreateScope(content string, inheritedVariableList map[string]*Variable, inheritedFunctionList map[string]*Function) *Scope {
	variableList := make(map[string]*Variable)
	functionList := make(map[string]*Function)

	if inheritedVariableList != nil {
		for key, variable := range inheritedVariableList {
			variableList[key] = variable
		}
	}

	if inheritedFunctionList != nil {
		for key, function := range inheritedFunctionList {
			functionList[key] = function
		}
	}

	return &Scope{isMainScope: false, initialContent: content, variableList: variableList, functionList: functionList}
}

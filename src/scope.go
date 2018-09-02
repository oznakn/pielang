package main

import (
	"regexp"
	"strings"
)

var MainScopeValueList []*Value

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

func (scope *Scope) IsVariableExists(variableName string) bool {
	_, ok := scope.variableList[variableName]
	return ok
}

func (scope *Scope) FindVariable(variableName string) *Variable {
	if variable, ok := scope.variableList[variableName]; ok {
		Debug("Scope", "Variable found", variableName)

		return variable
	}

	panic("Undefined Variable")
}

func (scope *Scope) IsFunctionExists(functionName string) bool {
	_, ok := scope.functionList[functionName]
	return ok
}

func (scope *Scope) FindFunction(functionName string) *Function {
	if function, ok := scope.functionList[functionName]; ok {
		Debug("Scope", "Function found", functionName)

		return function
	}

	panic("Undefined Function")
}

func (scope *Scope) AddVariable(variable *Variable) {
	scope.variableList[variable.variableName] = variable
	Debug("Scope", "New Variable", variable)
}

func (scope *Scope) AddFunction(function *Function) {
	scope.functionList[function.functionName] = function
	Debug("Scope", "New Function", function)
}

func (scope *Scope) runForMainScope() {
	var regex *regexp.Regexp

	regex = regexp.MustCompile(COMMENT_FIND_REGEX)
	scope.content = regex.ReplaceAllString(scope.content, `$1$2`)

	regex = regexp.MustCompile(NEWLINE_FIND_REGEX)
	scope.content = regex.ReplaceAllString(scope.content, `$1;$2`)

	if scope.content[len(scope.content)-1] == ';' {
		scope.content = scope.content[:len(scope.content)-1]
	}

	ReplaceStringsWithStringLiterals(scope)

	regex = regexp.MustCompile(FUNCTION_FIRST_LINE_FIND_REGEX)

	for len(regex.FindAllString(scope.content, 1)) > 0 {
		loc := regex.FindStringIndex(scope.content)

		startIndex := loc[0]
		endIndex := FindSiblingPosition(scope.content, loc[1]-1, "{", "}")
		content := scope.content[startIndex : endIndex+1]
		scope.content = scope.content[:startIndex] + scope.content[endIndex+1:]

		scope.AddFunction(ParseFunction(scope, content))
	}
}

func (scope *Scope) Run() {
	var regex *regexp.Regexp
	scope.content = scope.initialContent

	if scope.isMainScope {
		scope.runForMainScope()
	} else {
		regex = regexp.MustCompile(FUNCTION_FIRST_LINE_FIND_REGEX)

		if len(regex.FindAllString(scope.content, 1)) > 0 {
			panic("You cannot declare a function in a function")
		}
	}

	scope.lines = strings.Split(scope.content, ";")

	for _, line := range scope.lines {
		RunLineRunner(scope, line)
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

func CreateMainScope(content string) *Scope {
	scope := CreateScope(content, nil, nil)
	scope.SetAsMainScope()
	return scope
}

func FindEqualityValueOfString(scope *Scope, content string) *Value {
	if IsValidVariableName(content) && scope.IsVariableExists(content) {
		return scope.FindVariable(content).value
	} else if IsValueValid(content) {
		return CreateValue(content)
	} else if stringLiteral := ParseStringLiteral(scope, content); stringLiteral != nil {
		return stringLiteral.value
	}

	return CreateValueFromString(content)
}

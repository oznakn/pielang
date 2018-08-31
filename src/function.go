package main

import (
	"fmt"
	"strings"
)

type Function struct {
	scope           *Scope
	content         string
	functionName    string
	functionContent string
	parameters      []string
}

func (function *Function) Run(arguments []*Argument) {

}

func CreateFunction(inheritedScope *Scope, content string, functionName string, functionContent string, parameters []string) *Function {
	scope := CreateScope(functionContent, inheritedScope.variableList, inheritedScope.functionList)

	fmt.Println(functionContent)

	return &Function{scope, content, functionName, functionContent, parameters}
}

func ParseFunction(inheritedScope *Scope, content string) *Function {
	endOfFunctionWordIndex := strings.Index(content, "function")
	firstParametersContentIndex := strings.Index(content, "(")
	lastParametersContentIndex := strings.Index(content, ")")
	firstFunctionContentIndex := strings.Index(content, "{")

	if endOfFunctionWordIndex == -1 ||
		firstParametersContentIndex == -1 ||
		lastParametersContentIndex == -1 ||
		firstFunctionContentIndex == -1 {
		panic("Invalid Function Syntax")
	}

	functionName := strings.TrimSpace(content[endOfFunctionWordIndex+9 : firstParametersContentIndex])

	parameters := strings.Split(content[firstParametersContentIndex+1:lastParametersContentIndex], ",")

	for key, parameter := range parameters {
		parameters[key] = strings.TrimSpace(parameter)
	}

	functionContent := content[firstFunctionContentIndex+1 : len(content)-1]

	return CreateFunction(inheritedScope, content, functionName, functionContent, parameters)
}

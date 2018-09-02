package main

import (
	"regexp"
	"strings"
)

func findOperationsInArray(array []string) int {
	if index := FindStringInArray(array, "^"); index != -1 {
		return index
	} else if index := FindStringInArray(array, "*"); index != -1 {
		return index
	} else if index := FindStringInArray(array, "/"); index != -1 {
		return index
	} else if index := FindStringInArray(array, "%"); index != -1 {
		return index
	} else if index := FindStringInArray(array, "+"); index != -1 {
		return index
	} else if index := FindStringInArray(array, "-"); index != -1 {
		return index
	}

	return -1
}

type Expression struct {
	scope   *Scope
	content string
}

func (expression *Expression) Run() *Value {
	regex := regexp.MustCompile(FIND_OUTER_PARANTHESIS_GROUP_REGEX)

	expression.content = regex.ReplaceAllStringFunc(expression.content,
		func(newExpressionContent string) string {
			newExpressionContent = newExpressionContent[1 : len(newExpressionContent)-1]

			newExpression := CreateExpression(expression.scope, newExpressionContent)
			newValue := newExpression.Run()

			return newValue.GetAsString()
		})

	contents := strings.Split(expression.content, " ")
	parts := make([]string, 0)

	for _, value := range contents {
		value = strings.TrimSpace(value)

		if value != "" {
			parts = append(parts, value)
		}
	}

	for true {
		index := findOperationsInArray(parts)
		if index == -1 {
			break
		}

		operation := ParseOperation(expression.scope, parts[index-1:index+2])

		parts[index-1] = operation.Run().GetAsString()

		if len(parts) > index+2 {
			parts = append(parts[:index], parts[index+2:]...)
		} else {
			parts = parts[:index]
		}
	}

	result := FindEqualityValueOfString(expression.scope, parts[0])
	Debug("Expression result", result)

	return result
}

func CreateExpression(scope *Scope, content string) *Expression {
	Debug("Expression", "New expression:", content)
	return &Expression{scope, content}
}

package main

import (
	"regexp"
	"strconv"
)

type StringLiteral struct {
	index int
	value *Value
}

func (stringLiteral *StringLiteral) setIndex(index int) {
	stringLiteral.index = index
}

func (stringLiteral *StringLiteral) GetAsString() string {
	return "~" + strconv.Itoa(stringLiteral.index) + "~"
}

func (stringLiteral *StringLiteral) String() string {
	return stringLiteral.GetAsString() + " :" + stringLiteral.value.stringValue
}

func CreateStringLiteral(scope *Scope, value *Value) *StringLiteral {
	return &StringLiteral{index: -1, value: value}
}

var StringLiteralList = make([]*StringLiteral, 0)

func FindStringLiteral(scope *Scope, index int) *StringLiteral {
	if len(StringLiteralList) > index {
		return StringLiteralList[index]
	}

	return nil
}

func ParseStringLiteral(scope *Scope, content string) *StringLiteral {
	if content[0] == '~' && content[len(content)-1] == '~' {
		index, err := strconv.Atoi(content[1 : len(content)-1])

		if err == nil {
			Debug("StringLiteral", "String Literal Parsed", index)

			return FindStringLiteral(scope, index)
		}
	}

	return nil
}

func AddStringLiteral(scope *Scope, stringLiteral *StringLiteral) int {
	index := len(StringLiteralList)

	StringLiteralList = append(StringLiteralList, stringLiteral)
	stringLiteral.setIndex(index)

	Debug("StringLiteral", "New String Literal", stringLiteral.index, ":", stringLiteral.value)

	return index
}

func CreateAndAddStringLiteral(scope *Scope, value *Value) (*StringLiteral, int) {
	stringLiteral := CreateStringLiteral(scope, value)

	index := AddStringLiteral(scope, stringLiteral)

	return stringLiteral, index
}

func ReplaceStringsWithStringLiterals(scope *Scope) {
	if scope.isMainScope == false {
		panic("Replacing StringLiterals only works on MainScope")
	}

	regex := regexp.MustCompile(STRINGS_FIND_REGEX)

	scope.content = regex.ReplaceAllStringFunc(scope.content,
		func(decleration string) string {
			value := CreateValueFromString(decleration[1 : len(decleration)-1])

			stringLiteral, _ := CreateAndAddStringLiteral(scope, value)

			return stringLiteral.GetAsString()
		})

	Debug("StringLiteral", "New Content", scope.content)
}

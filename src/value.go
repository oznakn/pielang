package main

import (
	"strconv"
	"strings"
)

func convertStringToInt(s string) (int64, bool) {
	value, err := strconv.ParseInt(s, 10, 0)

	return value, err == nil
}

func convertStringToFloat(s string) (float64, bool) {
	value, err := strconv.ParseFloat(s, 64)

	return value, err == nil
}

func convertStringToBool(s string) (bool, bool) {
	value, err := strconv.ParseBool(s)

	return value, err == nil
}

type ValueType int

const (
	VALUE_TYPE_UNDEFINED ValueType = 0
	VALUE_TYPE_BOOL      ValueType = 1
	VALUE_TYPE_INT       ValueType = 2
	VALUE_TYPE_FLOAT     ValueType = 3
	VALUE_TYPE_STRING    ValueType = 4
)

type Value struct {
	valueType   ValueType
	boolValue   bool
	intValue    int64
	floatValue  float64
	stringValue string
}

func (value *Value) GetValueTypeString() string {
	switch value.valueType {
	case VALUE_TYPE_BOOL:
		return "bool"

	case VALUE_TYPE_INT:
		return "int"

	case VALUE_TYPE_FLOAT:
		return "float"

	case VALUE_TYPE_STRING:
		return "string"

	default:
		return "undefined"
	}
}

func (value *Value) GetAsString() string {
	switch value.valueType {
	case VALUE_TYPE_UNDEFINED:
		return "undefined"

	case VALUE_TYPE_BOOL:
		return strconv.FormatBool(value.boolValue)

	case VALUE_TYPE_INT:
		return strconv.FormatInt(value.intValue, 10)

	case VALUE_TYPE_FLOAT:
		return strconv.FormatFloat(value.floatValue, 'f', -1, 64)

	case VALUE_TYPE_STRING:
		return value.stringValue
	}

	return ""
}

func (value *Value) String() string {
	return value.GetAsString()
}

func CreateValueFromInt(value int64) *Value {
	return &Value{valueType: VALUE_TYPE_INT, intValue: value}
}

func CreateValueFromFloat(value float64) *Value {
	return &Value{valueType: VALUE_TYPE_FLOAT, floatValue: value}
}

func CreateValueFromBool(value bool) *Value {
	return &Value{valueType: VALUE_TYPE_BOOL, boolValue: value}
}

func CreateValueFromString(value string) *Value {
	return &Value{valueType: VALUE_TYPE_STRING, stringValue: value}
}

func CreateUndefinedValue() *Value {
	return &Value{valueType: VALUE_TYPE_UNDEFINED}
}

func CreateValue(decleration string) *Value {
	decleration = strings.TrimSpace(decleration)

	if value, ok := convertStringToInt(decleration); ok {
		return CreateValueFromInt(value)
	} else if value, ok := convertStringToFloat(decleration); ok {
		return CreateValueFromFloat(value)
	} else if value, ok := convertStringToBool(decleration); ok {
		return CreateValueFromBool(value)
	} else if decleration[0] == '"' && decleration[len(decleration)-1] == '"' {
		return CreateValueFromString(decleration[1 : len(decleration)-1])
	}

	return CreateUndefinedValue()
}

func IsValueValid(decleration string) bool {
	decleration = strings.TrimSpace(decleration)

	if _, ok := convertStringToInt(decleration); ok {
		return true
	} else if _, ok := convertStringToFloat(decleration); ok {
		return true
	} else if _, ok := convertStringToBool(decleration); ok {
		return true
	} else if decleration[0] == '"' && decleration[len(decleration)-1] == '"' {
		return true
	}

	return false
}

func ParseValue(scope *Scope, content string) *Value {
	content = strings.TrimSpace(content)

	return CreateValue(content)
}

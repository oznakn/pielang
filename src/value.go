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

func (value *Value) applyAdditionOperator(value2 *Value) *Value {
	if value.valueType == VALUE_TYPE_INT {
		if value2.valueType == VALUE_TYPE_INT {
			return CreateValueFromInt(value.intValue + value2.intValue)
		} else if value2.valueType == VALUE_TYPE_FLOAT {
			return CreateValueFromFloat(float64(value.intValue) + value2.floatValue)
		}
	} else if value.valueType == VALUE_TYPE_FLOAT {
		if value2.valueType == VALUE_TYPE_INT {
			return CreateValueFromFloat(value.floatValue + float64(value2.intValue))
		} else if value2.valueType == VALUE_TYPE_FLOAT {
			return CreateValueFromFloat(value.floatValue + value2.floatValue)
		}
	}

	return CreateValueFromString(value.GetAsString() + value2.GetAsString())
}

func (value *Value) applySubstractionOperator(value2 *Value) *Value {
	if value.valueType == VALUE_TYPE_INT {
		if value2.valueType == VALUE_TYPE_INT {
			return CreateValueFromInt(value.intValue - value2.intValue)
		} else if value2.valueType == VALUE_TYPE_FLOAT {
			return CreateValueFromFloat(float64(value.intValue) - value2.floatValue)
		}
	} else if value.valueType == VALUE_TYPE_FLOAT {
		if value2.valueType == VALUE_TYPE_INT {
			return CreateValueFromFloat(value.floatValue - float64(value2.intValue))
		} else if value2.valueType == VALUE_TYPE_FLOAT {
			return CreateValueFromFloat(value.floatValue - value2.floatValue)
		}
	}

	panic("Invalid operator")
}

func (value *Value) applyMultiplicationOperator(value2 *Value) *Value {
	if value.valueType == VALUE_TYPE_INT {
		if value2.valueType == VALUE_TYPE_INT {
			return CreateValueFromInt(value.intValue * value2.intValue)
		} else if value2.valueType == VALUE_TYPE_FLOAT {
			return CreateValueFromFloat(float64(value.intValue) * value2.floatValue)
		}
	} else if value.valueType == VALUE_TYPE_FLOAT {
		if value2.valueType == VALUE_TYPE_INT {
			return CreateValueFromFloat(value.floatValue * float64(value2.intValue))
		} else if value2.valueType == VALUE_TYPE_FLOAT {
			return CreateValueFromFloat(value.floatValue * value2.floatValue)
		}
	}

	panic("Invalid operator")
}

func (value *Value) applyDivisionOperator(value2 *Value) *Value {
	if value.valueType == VALUE_TYPE_INT {
		if value2.valueType == VALUE_TYPE_INT {
			return CreateValueFromInt(value.intValue / value2.intValue)
		} else if value2.valueType == VALUE_TYPE_FLOAT {
			return CreateValueFromFloat(float64(value.intValue) / value2.floatValue)
		}
	} else if value.valueType == VALUE_TYPE_FLOAT {
		if value2.valueType == VALUE_TYPE_INT {
			return CreateValueFromFloat(value.floatValue / float64(value2.intValue))
		} else if value2.valueType == VALUE_TYPE_FLOAT {
			return CreateValueFromFloat(value.floatValue / value2.floatValue)
		}
	}

	panic("Invalid operator")
}

func (value *Value) ApplyOperator(value2 *Value, operator string) *Value {
	switch operator {
	case "+":
		return value.applyAdditionOperator(value2)
	case "-":
		return value.applySubstractionOperator(value2)
	case "*":
		return value.applyMultiplicationOperator(value2)
	case "/":
		return value.applyDivisionOperator(value2)
	}

	panic("Invalid operator")
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

func IsValidValue(decleration string) bool {
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

func ParseValue(scope *Scope, decleration string) *Value {
	decleration = strings.TrimSpace(decleration)

	if IsValidVariableName(decleration) && scope.IsVariableExists(decleration) {
		return scope.FindVariable(decleration).value
	} else if IsValidValue(decleration) {
		return CreateValue(decleration)
	} else if strings.ContainsAny(decleration, "+-*/") {
		return ParseStatement(scope, decleration)
	} else if decleration[0] == '~' && decleration[len(decleration)-1] == '~' {
		decleration = decleration[1 : len(decleration)-1]

		valueIndex, err := strconv.Atoi(decleration)

		if err != nil {
			panic(err)
		}

		Debug("Value", "String Literal Resolved", valueIndex, MainScopeValueList[valueIndex])

		return MainScopeValueList[valueIndex]
	}

	return CreateUndefinedValue()
}

func ParseStatement(scope *Scope, decleration string) *Value {
	parts1 := strings.Split(decleration, "-")

	var value1 *Value

	for key1, part1 := range parts1 {
		part1 = strings.TrimSpace(part1)
		parts2 := strings.Split(part1, "+")
		var value2 *Value

		for key2, part2 := range parts2 {
			part2 = strings.TrimSpace(part2)
			parts3 := strings.Split(part2, "/")
			var value3 *Value

			for key3, part3 := range parts3 {
				part3 = strings.TrimSpace(part3)
				parts4 := strings.Split(part3, "*")
				var value4 *Value

				for key4, part4 := range parts4 {
					part4 = strings.TrimSpace(part4)

					if key4 == 0 {
						value4 = ParseValue(scope, part4)
					} else {
						value4 = value4.applyMultiplicationOperator(ParseValue(scope, part4))
					}
				}

				if key3 == 0 {
					value3 = value4
				} else {
					value3 = value3.applyDivisionOperator(value4)
				}
			}

			if key2 == 0 {
				value2 = value3
			} else {
				value2 = value2.applyAdditionOperator(value3)
			}
		}

		if key1 == 0 {
			value1 = value2
		} else {
			value1 = value1.applySubstractionOperator(value2)
		}
	}

	return value1
}

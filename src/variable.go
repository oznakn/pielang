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

type VariableType int

const (
	VARIABLE_TYPE_BOOL   VariableType = 0
	VARIABLE_TYPE_INT    VariableType = 1
	VARIABLE_TYPE_FLOAT  VariableType = 2
	VARIABLE_TYPE_STRING VariableType = 3
)

type Variable struct {
	variableType VariableType
	boolValue    bool
	intValue     int64
	floatValue   float64
	stringValue  string
}

func (variable *Variable) GetVariableTypeString() string {
	switch variable.variableType {
	case VARIABLE_TYPE_BOOL:
		return "bool"

	case VARIABLE_TYPE_INT:
		return "int"

	case VARIABLE_TYPE_FLOAT:
		return "float"

	case VARIABLE_TYPE_STRING:
		return "string"

	default:
		return "undefined"
	}
}

func (variable *Variable) getAsString() string {
	switch variable.variableType {
	case VARIABLE_TYPE_BOOL:
		return strconv.FormatBool(variable.boolValue)

	case VARIABLE_TYPE_INT:
		return strconv.FormatInt(variable.intValue, 10)

	case VARIABLE_TYPE_FLOAT:
		return strconv.FormatFloat(variable.floatValue, 'f', -1, 64)

	case VARIABLE_TYPE_STRING:
		return variable.stringValue
	}

	return ""
}

func (variable *Variable) String() string {
	return variable.getAsString()
}

func (variable *Variable) applyAdditionOperator(variable2 *Variable) *Variable {
	if variable.variableType == VARIABLE_TYPE_INT {
		if variable2.variableType == VARIABLE_TYPE_INT {
			return createVariableFromInt(variable.intValue + variable2.intValue)
		} else if variable2.variableType == VARIABLE_TYPE_FLOAT {
			return createVariableFromFloat(float64(variable.intValue) + variable2.floatValue)
		}
	} else if variable.variableType == VARIABLE_TYPE_FLOAT {
		if variable2.variableType == VARIABLE_TYPE_INT {
			return createVariableFromFloat(variable.floatValue + float64(variable2.intValue))
		} else if variable2.variableType == VARIABLE_TYPE_FLOAT {
			return createVariableFromFloat(variable.floatValue + variable2.floatValue)
		}
	}

	return createVariableFromString(variable.getAsString() + variable2.getAsString())
}

func (variable *Variable) applySubstractionOperator(variable2 *Variable) *Variable {
	if variable.variableType == VARIABLE_TYPE_INT {
		if variable2.variableType == VARIABLE_TYPE_INT {
			return createVariableFromInt(variable.intValue - variable2.intValue)
		} else if variable2.variableType == VARIABLE_TYPE_FLOAT {
			return createVariableFromFloat(float64(variable.intValue) - variable2.floatValue)
		}
	} else if variable.variableType == VARIABLE_TYPE_FLOAT {
		if variable2.variableType == VARIABLE_TYPE_INT {
			return createVariableFromFloat(variable.floatValue - float64(variable2.intValue))
		} else if variable2.variableType == VARIABLE_TYPE_FLOAT {
			return createVariableFromFloat(variable.floatValue - variable2.floatValue)
		}
	}

	panic("Invalid operator")
}

func (variable *Variable) applyMultiplicationOperator(variable2 *Variable) {

}

func (variable *Variable) applyDivisionOperator(variable2 *Variable) {

}

func (variable *Variable) ApplyOperator(variable2 *Variable, operator string) *Variable {
	switch operator {
	case "+":
		return variable.applyAdditionOperator(variable2)
	case "-":
		return variable.applySubstractionOperator(variable2)
	}

	panic("Invalid operator")
}

func createVariableFromInt(value int64) *Variable {
	return &Variable{variableType: VARIABLE_TYPE_INT, intValue: value}
}

func createVariableFromFloat(value float64) *Variable {
	return &Variable{variableType: VARIABLE_TYPE_FLOAT, floatValue: value}
}

func createVariableFromBool(value bool) *Variable {
	return &Variable{variableType: VARIABLE_TYPE_BOOL, boolValue: value}
}

func createVariableFromString(value string) *Variable {
	return &Variable{variableType: VARIABLE_TYPE_STRING, stringValue: value}
}

func CreateVariable(decleration string) *Variable {
	decleration = strings.TrimSpace(decleration)

	if value, ok := convertStringToInt(decleration); ok {
		return createVariableFromInt(value)
	} else if value, ok := convertStringToFloat(decleration); ok {
		return createVariableFromFloat(value)

	} else if value, ok := convertStringToBool(decleration); ok {
		return createVariableFromBool(value)
	}
	return createVariableFromString(decleration)
}

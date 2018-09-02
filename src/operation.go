package main

import (
	"math"
	"strings"
)

type OperationType int

const (
	OPERATION_TYPE_NONE           OperationType = 0
	OPERATION_TYPE_ADDITION       OperationType = 1
	OPERATION_TYPE_SUBSTRACTION   OperationType = 2
	OPERATION_TYPE_MULTIPLICATION OperationType = 3
	OPERATION_TYPE_DIVISION       OperationType = 4
	OPERATION_TYPE_MODE           OperationType = 5
	OPERATION_TYPE_POWER          OperationType = 6
)

type Operation struct {
	scope         *Scope
	firstValue    *Value
	lastValue     *Value
	operationType OperationType
}

func (operation *Operation) runAddition() *Value {
	if operation.firstValue.valueType == VALUE_TYPE_INT {
		if operation.lastValue.valueType == VALUE_TYPE_INT {
			return CreateValueFromInt(operation.firstValue.intValue + operation.lastValue.intValue)
		} else if operation.lastValue.valueType == VALUE_TYPE_FLOAT {
			return CreateValueFromFloat(float64(operation.firstValue.intValue) + operation.lastValue.floatValue)
		}
	} else if operation.firstValue.valueType == VALUE_TYPE_FLOAT {
		if operation.lastValue.valueType == VALUE_TYPE_INT {
			return CreateValueFromFloat(operation.firstValue.floatValue + float64(operation.lastValue.intValue))
		} else if operation.lastValue.valueType == VALUE_TYPE_FLOAT {
			return CreateValueFromFloat(operation.firstValue.floatValue + operation.lastValue.floatValue)
		}
	}

	return CreateValueFromString(operation.firstValue.GetAsString() + operation.lastValue.GetAsString())
}

func (operation *Operation) runSubstraction() *Value {
	if operation.firstValue.valueType == VALUE_TYPE_INT {
		if operation.lastValue.valueType == VALUE_TYPE_INT {
			return CreateValueFromInt(operation.firstValue.intValue - operation.lastValue.intValue)
		} else if operation.lastValue.valueType == VALUE_TYPE_FLOAT {
			return CreateValueFromFloat(float64(operation.firstValue.intValue) - operation.lastValue.floatValue)
		}
	} else if operation.firstValue.valueType == VALUE_TYPE_FLOAT {
		if operation.lastValue.valueType == VALUE_TYPE_INT {
			return CreateValueFromFloat(operation.firstValue.floatValue - float64(operation.lastValue.intValue))
		} else if operation.lastValue.valueType == VALUE_TYPE_FLOAT {
			return CreateValueFromFloat(operation.firstValue.floatValue - operation.lastValue.floatValue)
		}
	}

	panic("Invalid operator")
}

func (operation *Operation) runMultiplication() *Value {
	if operation.firstValue.valueType == VALUE_TYPE_INT {
		if operation.lastValue.valueType == VALUE_TYPE_INT {
			return CreateValueFromInt(operation.firstValue.intValue * operation.lastValue.intValue)
		} else if operation.lastValue.valueType == VALUE_TYPE_FLOAT {
			return CreateValueFromFloat(float64(operation.firstValue.intValue) * operation.lastValue.floatValue)
		}
	} else if operation.firstValue.valueType == VALUE_TYPE_FLOAT {
		if operation.lastValue.valueType == VALUE_TYPE_INT {
			return CreateValueFromFloat(operation.firstValue.floatValue * float64(operation.lastValue.intValue))
		} else if operation.lastValue.valueType == VALUE_TYPE_FLOAT {
			return CreateValueFromFloat(operation.firstValue.floatValue * operation.lastValue.floatValue)
		}
	}

	panic("Invalid operator")
}

func (operation *Operation) runDivision() *Value {
	if operation.firstValue.valueType == VALUE_TYPE_INT {
		if operation.lastValue.valueType == VALUE_TYPE_INT {
			return CreateValueFromInt(operation.firstValue.intValue / operation.lastValue.intValue)
		} else if operation.lastValue.valueType == VALUE_TYPE_FLOAT {
			return CreateValueFromFloat(float64(operation.firstValue.intValue) / operation.lastValue.floatValue)
		}
	} else if operation.firstValue.valueType == VALUE_TYPE_FLOAT {
		if operation.lastValue.valueType == VALUE_TYPE_INT {
			return CreateValueFromFloat(operation.firstValue.floatValue / float64(operation.lastValue.intValue))
		} else if operation.lastValue.valueType == VALUE_TYPE_FLOAT {
			return CreateValueFromFloat(operation.firstValue.floatValue / operation.lastValue.floatValue)
		}
	}

	panic("Invalid operator")
}

func (operation *Operation) runMode() *Value {
	if operation.firstValue.valueType == VALUE_TYPE_INT {
		if operation.lastValue.valueType == VALUE_TYPE_INT {
			return CreateValueFromInt(operation.firstValue.intValue % operation.lastValue.intValue)
		}
	} else if operation.firstValue.valueType == VALUE_TYPE_FLOAT {
		if operation.lastValue.valueType == VALUE_TYPE_INT {
			return CreateValueFromInt(int64(operation.firstValue.floatValue) % operation.lastValue.intValue)
		}
	}

	panic("Invalid operator")
}

func (operation *Operation) runPower() *Value {
	if operation.firstValue.valueType == VALUE_TYPE_INT {
		if operation.lastValue.valueType == VALUE_TYPE_INT {
			result := math.Pow(float64(operation.firstValue.intValue), float64(operation.lastValue.intValue))

			return CreateValueFromInt(int64(result))
		} else if operation.lastValue.valueType == VALUE_TYPE_FLOAT {
			result := math.Pow(float64(operation.firstValue.intValue), operation.lastValue.floatValue)

			return CreateValueFromFloat(result)
		}
	} else if operation.firstValue.valueType == VALUE_TYPE_FLOAT {
		if operation.lastValue.valueType == VALUE_TYPE_INT {
			result := math.Pow(operation.firstValue.floatValue, float64(operation.lastValue.intValue))

			return CreateValueFromFloat(result)
		} else if operation.lastValue.valueType == VALUE_TYPE_FLOAT {
			result := math.Pow(operation.firstValue.floatValue, operation.lastValue.floatValue)

			return CreateValueFromFloat(result)
		}
	}

	panic("Invalid operator")
}

func (operation *Operation) Run() *Value {
	switch operation.operationType {
	case OPERATION_TYPE_ADDITION:
		return operation.runAddition()
	case OPERATION_TYPE_SUBSTRACTION:
		return operation.runSubstraction()
	case OPERATION_TYPE_MULTIPLICATION:
		return operation.runMultiplication()
	case OPERATION_TYPE_DIVISION:
		return operation.runDivision()
	case OPERATION_TYPE_MODE:
		return operation.runMode()
	case OPERATION_TYPE_POWER:
		return operation.runPower()
	}

	panic("Invalid operator")
}

func CreateOperation(scope *Scope, firstValue *Value, lastValue *Value, operationType OperationType) *Operation {
	Debug("Operation", "New operation:", firstValue, lastValue, operationType)

	return &Operation{scope, firstValue, lastValue, operationType}
}

func ParseOperation(scope *Scope, contents []string) *Operation {
	operationType := OPERATION_TYPE_NONE

	if len(contents) == 3 {
		firstValue := FindEqualityValueOfString(scope, contents[0])
		lastValue := FindEqualityValueOfString(scope, contents[2])

		operator := strings.TrimSpace(contents[1])
		switch operator {
		case "+":
			operationType = OPERATION_TYPE_ADDITION
		case "-":
			operationType = OPERATION_TYPE_SUBSTRACTION
		case "*":
			operationType = OPERATION_TYPE_MULTIPLICATION
		case "/":
			operationType = OPERATION_TYPE_DIVISION
		case "%":
			operationType = OPERATION_TYPE_MODE
		case "^":
			operationType = OPERATION_TYPE_POWER
		default:
			panic("Invalid Operator: " + operator)
		}

		if operationType != OPERATION_TYPE_NONE {
			return CreateOperation(scope, firstValue, lastValue, operationType)
		}
	}

	panic("Invalid Operation")
}

func ParseOperationWithString(scope *Scope, content string) *Operation {
	content = strings.TrimSpace(content)
	contents := strings.Split(content, " ")

	return ParseOperation(scope, contents)
}

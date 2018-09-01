package main

type SystemFunction func(values []*Value) *Value
type SystemFunctionMap map[string]SystemFunction
type SystemModuleMap map[string]SystemFunctionMap

func systemPrintln() {

}

var systemFunctions = SystemFunctionMap{
	"println": func(values []*Value) *Value {
		LogNewLine(values[0])
		return CreateUndefinedValue()
	},
	"print": func(values []*Value) *Value {
		Log(values[0])
		return CreateUndefinedValue()
	},
}

var systemModules = SystemModuleMap{
	"console": {
		"println": func(values []*Value) *Value {
			LogNewLine(values[0])
			return CreateUndefinedValue()
		},
	},
}

func IsSystemFunctionExists(moduleName string, functionName string) bool {
	if moduleName != "" {
		if _, ok := systemModules[moduleName][functionName]; ok {
			return true
		}
	} else {
		if _, ok := systemFunctions[functionName]; ok {
			return true
		}
	}

	return false
}

func RunSystemFunction(moduleName string, functionName string, values []*Value) *Value {
	if moduleName != "" {
		if function, ok := systemModules[moduleName][functionName]; ok {
			return function(values)
		}
	} else {
		if function, ok := systemFunctions[functionName]; ok {
			return function(values)
		}
	}

	panic("Undefined function")
}

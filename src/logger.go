package main

import (
	"fmt"
	"strings"

	"github.com/fatih/color"
)

var bluePrinter = color.New(color.FgBlue)
var hiBluePrinter = color.New(color.FgHiBlue)

var willBeBrinted = ""

func joinInterfaces(values []interface{}) string {
	valueList := make([]string, len(values))
	for i, v := range values {
		valueList[i] = fmt.Sprintf("%s", v)
	}
	return strings.Join(valueList, " ")
}

func Debug(context string, content ...interface{}) {
	fmt.Print(">")
	hiBluePrinter.Print(" [" + context + "] ")
	bluePrinter.Println(content...)
}

func Log(content ...interface{}) {
	if DEBUG_MODE {
		willBeBrinted += joinInterfaces(content)
	} else {
		fmt.Print(content...)
	}
}

func LogNewLine(content ...interface{}) {
	if DEBUG_MODE {
		willBeBrinted += joinInterfaces(content) + "\n"
	} else {
		fmt.Println(content...)
	}
}

func PostLog() {
	if DEBUG_MODE {
		fmt.Println(willBeBrinted)
	}
}

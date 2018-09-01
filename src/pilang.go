package main

import (
	"os"
)

func main() {
	var fileName string

	if len(os.Args) >= 2 {
		fileName = os.Args[1]
	} else {
		fileName = DEFAULT_FILE_NAME
	}

	fileContent := ReadFile(fileName)

	scope := CreateMainScope(fileContent)
	scope.Run()
	PostLog()
}

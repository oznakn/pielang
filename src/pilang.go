package main

import (
	"fmt"
	"os"
	"time"
)

func main() {
	var fileName string

	if len(os.Args) >= 2 {
		fileName = os.Args[1]
	} else {
		fileName = DEFAULT_FILE_NAME
	}

	fileContent := ReadFile(fileName)

	start := time.Now()

	scope := CreateMainScope(fileContent)
	scope.Run()
	PostLog()

	elapsed := time.Since(start)
	fmt.Println("Program finished in:", elapsed.Seconds()*1000, "ms")
}

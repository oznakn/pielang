package main

// "os"

func main() {
	/* if len(os.Args) < 2 {
	   return
	 } */

	fileName := "examples/sample.plang" //os.Args[1]

	fileContent := ReadFile(fileName)

	scope := CreateScope(fileContent, nil, nil)
	scope.SetAsMainScope()
	scope.Run()
}

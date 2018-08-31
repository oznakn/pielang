package main

import (
	"io/ioutil"
)

func ReadFile(fileName string) string {
	data, err := ioutil.ReadFile(fileName)

	if err != nil {
		panic(err)
	}

	return string(data)
}

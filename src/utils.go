package main

import (
	"strings"
)

func FindSiblingPosition(content string, startIndex int, char string, siblingChar string) int {
	var index, siblingIndex, lastIndex int
	i := 1
	startIndex += 1

	for i != 0 {
		index = strings.Index(content[startIndex:], char)
		siblingIndex = strings.Index(content[startIndex:], siblingChar)

		if index > -1 && siblingIndex > index {
			if siblingIndex < 0 {
				siblingIndex = 1
			}

			lastIndex = siblingIndex + startIndex
			i += strings.Count(content[startIndex:lastIndex], char)
			startIndex = lastIndex
		} else if siblingIndex > -1 {
			if index < 0 {
				index = 1
			}

			lastIndex = index + startIndex
			i -= strings.Count(content[startIndex:lastIndex], siblingChar)
			startIndex = lastIndex
		}
	}

	return startIndex - 1
}

func FindStringInArray(array []string, value string) int {
	for k, v := range array {
		if v == value {
			return k
		}
	}

	return -1
}

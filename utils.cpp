#include "utils.h"

#include <string>
#include <iostream>

#include "stringutils.h"

size_t Utils::findSiblingPosition(std::string content, size_t startIndex, char normalChar, char siblingChar) {
    size_t index, siblingIndex, lastIndex;

    size_t i = 1;
    startIndex++;

    while (i != 0) {
        index = content.find(normalChar, startIndex);
        siblingIndex = content.find(siblingChar, startIndex);


        if (siblingIndex != std::string::npos && index > siblingIndex) {
            if (index == std::string::npos) {
                index = 1;
            }

            lastIndex = index + startIndex;
            i -= StringUtils::count(StringUtils::substring(content, startIndex, lastIndex), siblingChar);
            startIndex = lastIndex;
        }
        else if (index != std::string::npos) {
            if (siblingIndex == std::string::npos) {
                siblingIndex = 1;
            }

            lastIndex = siblingIndex + startIndex;
            i += StringUtils::count(StringUtils::substring(content, startIndex, lastIndex), normalChar);
            startIndex = lastIndex;
        }
    }

    return startIndex - 1;
}


size_t Utils::findStringInVector(StringList* contents, std::string s) {
    for (size_t i = 0; i < contents->size(); i++) {
        if (contents->at(i) == s) {
            return i;
        }
    }

    return -1;
}
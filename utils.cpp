#include "utils.h"

#include <string>
#include <iostream>

#include "stringutils.h"

using namespace std;

size_t Utils::findSiblingPosition(string content, size_t startIndex, char normalChar, char siblingChar) {
    size_t index, siblingIndex, lastIndex;

    size_t i = 1;
    startIndex++;

    while (i != 0) {
        index = content.find(normalChar, startIndex);
        siblingIndex = content.find(siblingChar, startIndex);


        if (siblingIndex != string::npos && index > siblingIndex) {
            if (index == string::npos) {
                index = 1;
            }

            lastIndex = index + startIndex;
            i -= StringUtils::count(StringUtils::substring(content, startIndex, lastIndex), siblingChar);
            startIndex = lastIndex;
        }
        else if (index != string::npos) {
            if (siblingIndex == string::npos) {
                siblingIndex = 1;
            }

            lastIndex = siblingIndex + startIndex;
            i += StringUtils::count(StringUtils::substring(content, startIndex, lastIndex), normalChar);
            startIndex = lastIndex;
        }
    }

    return startIndex - 1;
}

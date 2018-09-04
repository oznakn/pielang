#include "stringutils.h"

#include <iostream>
#include <cctype>
#include <algorithm>
#include <string>
#include <vector>

bool StringUtils::isSpaceLike(char c) {
    return isspace(c);
}

bool StringUtils::isNotSpaceLike(char c) {
    return !StringUtils::isSpaceLike(c);
}

bool StringUtils::isBothSpaceLike(char c1, char c2) {
    return StringUtils::isSpaceLike(c1) && StringUtils::isSpaceLike(c2);
}

bool StringUtils::isBothNotSpaceLike(char c1, char c2) {
    return !StringUtils::isSpaceLike(c1) && !StringUtils::isSpaceLike(c2);
}

bool StringUtils::contains(std::string s, std::string i) {
    return s.find(i) != std::string::npos;
}

size_t StringUtils::count(std::string s, std::string i) {
    return StringUtils::count(s, i.at(0));
}

size_t StringUtils::count(std::string s, char c) {
    return std::count(s.begin(), s.end(), c);
}

std::string StringUtils::substring(std::string s, size_t startPosition, size_t endPosition) {
    return s.substr(startPosition, endPosition - startPosition);
}

std::string StringUtils::cutMiddle(std::string s, size_t middlePartStart, size_t secondPartStart) {
    return StringUtils::substring(s, 0, middlePartStart) + StringUtils::substring(s, secondPartStart, s.length());
}

std::string StringUtils::replaceMiddle(std::string s, std::string r, size_t middlePartStart, size_t secondPartStart) {
    return StringUtils::substring(s, 0, middlePartStart) + r + StringUtils::substring(s, secondPartStart, s.length());
}

std::string StringUtils::ltrim(std::string s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), StringUtils::isNotSpaceLike));

    return s;
}

std::string StringUtils::rtrim(std::string s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), StringUtils::isNotSpaceLike).base(), s.end());

    return s;
}

std::string StringUtils::trim(std::string s) {
    return rtrim(ltrim(s));
}

std::vector<std::string>* StringUtils::split(std::string s, std::string d) {
    size_t startIndex = 0;
    size_t index;

    auto list = new std::vector<std::string>;

    if (std::string(1, s.at(s.length() -1)) != d) {
        s += d;
    }

    while((index = s.find(d, startIndex)) != std::string::npos) {
        list->push_back(StringUtils::substring(s, startIndex, index));

        startIndex = index + 1;
    }

    return list;
}

std::string StringUtils::replaceMultipleSpacesWithOne(std::string s) {
    std::string::iterator it = std::unique(s.begin(), s.end(), StringUtils::isBothSpaceLike);
    s.erase(it, s.end());

    return s;
}


std::string StringUtils::replaceMultipleCharsWithOne(std::string s, const char c) {
    std::string::iterator it = std::unique(s.begin(), s.end(), [c](char c1, char c2) {
        return (c1 == c2) && (c1 == c);
    });
    s.erase(it, s.end());

    return s;
}

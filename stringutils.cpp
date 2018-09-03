#include "stringutils.h"

#include <iostream>
#include <cctype>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

bool StringUtils::contains(string s, string i) {
    return s.find(i) != string::npos;
}

size_t StringUtils::count(string s, string i) {
    return StringUtils::count(s, i.at(0));
}

size_t StringUtils::count(string s, char c) {
    return std::count(s.begin(), s.end(), c);
}

string StringUtils::substring(string s, size_t startPosition, size_t endPosition) {
    return s.substr(startPosition, endPosition - startPosition);
}

string StringUtils::cutMiddle(string s, size_t middlePartStart, size_t secondPartStart) {
    return StringUtils::substring(s, 0, middlePartStart) + StringUtils::substring(s, secondPartStart, s.length());
}

string StringUtils::replaceMiddle(string s, string r, size_t middlePartStart, size_t secondPartStart) {
    return StringUtils::substring(s, 0, middlePartStart) + r + StringUtils::substring(s, secondPartStart, s.length());
}

string StringUtils::ltrim(string s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));

    return s;
}

string StringUtils::rtrim(string s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());

    return s;
}

string StringUtils::trim(string s) {
    s = ltrim(s);
    s = rtrim(s);

    return s;
}

vector<string>* StringUtils::split(string s, string d) {
    size_t startIndex = 0;
    size_t index = string::npos;

    vector<string>* list = new vector<string>;

    if (string(1, s.at(s.length() -1)) != d) {
        s += d;
    }

    while((index = s.find(d, startIndex)) != string::npos) {
        list->push_back(StringUtils::substring(s, startIndex, index));

        startIndex = index + 1;
    }

    return list;
}

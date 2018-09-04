#include "variable.h"

#include <string.h>

#include "stringutils.h"

bool Variable::isValidVariableName(std::string s) {
    s = StringUtils::trim(s);

    if (s.length() > 0) {
        if (!std::isalpha(s.at(0))) {
            return false;
        }
        else if (s.length() > 1) {
            for (size_t i = 1; i < s.length(); i++) {
                if (!std::isalnum(s.at(i))) return false;
            }
        }
    }
    else {
        return false;
    }

    return true;
}
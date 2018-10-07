#ifndef PILANG_SYSTEM_H
#define PILANG_SYSTEM_H

#include <string>
#include <vector>
#include <unordered_map>

#include "definitions.h"

class System {
public:
    static Scope* mainScope;
    static void init(Scope*);
};


#endif //PILANG_SYSTEM_H

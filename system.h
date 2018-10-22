#ifndef PIELANG_SYSTEM_H
#define PIELANG_SYSTEM_H

#include <string>
#include <vector>
#include <unordered_map>

#include "definitions.h"

class System {
public:
    static Scope* mainScope;
    static void init(Scope*);
    static void initForScope(Scope*);
};


#endif //PIELANG_SYSTEM_H

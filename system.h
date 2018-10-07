#ifndef PILANG_SYSTEM_H
#define PILANG_SYSTEM_H

#include <string>
#include <vector>
#include <unordered_map>

class Scope;

class System {
public:
    static void init(Scope*);
};

#endif //PILANG_SYSTEM_H

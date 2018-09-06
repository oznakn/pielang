#ifndef PILANG_SYSTEM_H
#define PILANG_SYSTEM_H

#include <string>
#include <vector>
#include <unordered_map>

class SystemFunction;

class System {
private:
    // std::unordered_map<std::string, Variable*>* mSystemVariableMap;
    static std::unordered_map<std::string, SystemFunction*>* systemFunctionMap;

public:
    static void init();
    static bool hasSystemFunction(std::string);
    static SystemFunction* getSystemFunction(std::string);
};

#endif //PILANG_SYSTEM_H

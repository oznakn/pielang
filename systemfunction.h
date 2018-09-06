#ifndef PILANG_SYSTEMFUNCTION_H
#define PILANG_SYSTEMFUNCTION_H

#include <string>
#include <vector>

class Value;

class SystemFunction {
public:
    SystemFunction();
    virtual Value* run(std::vector<Value*>*);
};

#endif //PILANG_SYSTEMFUNCTION_H

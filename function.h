#ifndef PILANG_FUNCTION_H
#define PILANG_FUNCTION_H

#include <string>
#include <vector>

class Value;

class Function {
public:
    virtual size_t getParameterCount();
    virtual std::string getFunctionName();
    virtual Value* run(std::vector<Value*>*);
};


#endif //PILANG_FUNCTION_H

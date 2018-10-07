#ifndef PILANG_FUNCTION_H
#define PILANG_FUNCTION_H

#include <string>
#include <vector>

#include "definitions.h"

class Function {
private:
    ObjectList* mLinkedObjectList = nullptr;

public:
    virtual ~Function();

    virtual std::string getFunctionName();
    virtual Value* run(ValueList*);

    const void linkWithObject(Object*);
    const void unlinkWithObject(Object*);
    const void deleteIfNotLinked();
};

#endif //PILANG_FUNCTION_H

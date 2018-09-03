#ifndef PILANG_FUNCTION_H
#define PILANG_FUNCTION_H

#include <string>
#include <vector>

using namespace std;

class Scope;

class Function {
private:
    string mFunctionName;
    string mContent;
    Scope* mScope;
    vector<string>* mParameterList;

    void init(string, string, vector<string>*, Scope*);

public:
    Function(string, Scope*);
    Function(string, string, vector<string>*, Scope*);

    string getFunctionName();
};

#endif //PILANG_FUNCTION_H

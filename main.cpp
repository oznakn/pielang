#include <iostream>
#include <ctime>

#include "scope.h"

using namespace std;

int main() {
    clock_t begin = clock();

    Scope* scope = new Scope(
            "function name(a, b, c) {sdf}\n" \
            "\"selam\"" \
            );

    scope->setAsMainScope();
    scope->run();

    clock_t end = clock();

    cout << endl << "Program finished in: " << (double(end - begin) / 1000) << endl;

    return 0;
}

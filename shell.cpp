#include "shell.h"

#include <iostream>
#include <string>

#include "definitions.h"
#include "value.h"
#include "expression.h"
#include "variable.h"
#include "system.h"
#include "scope.h"

void Shell::start() {
	Scope* scope = new Scope("");
    scope->setAsMainScope();

    System::init(scope);

	std::string line;
	
	while(std::getline (std::cin, line)) {
		std::cin >> line;

		std::cout << line;

		auto expression = new Expression(scope, line);
    	expression->run();
	}
}
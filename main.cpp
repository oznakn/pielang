/**
 * @author Ozan Akın
 * Pilang Interpretter
 */

#include <iostream>
#include <string>
#include <fstream>

#include "logger.h"
#include "system.h"
#include "scope.h"
#include "value.h"
#include "expression.h"
#include "stringutils.h"

std::string readFile(std::string & fileName) {
    std::ifstream in(fileName, std::ios::in | std::ios::binary);
    if (in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return(contents);
    }

    return nullptr;
}

std::string findWorkingDir() {
    std::string dir = __FILE__;
    size_t index = dir.find_last_of('\\');

    if (index != std::string::npos) {
        return StringUtils::replaceMiddle(dir, "", index, dir.length());
    }

    return dir;
}

int main() {
    Logger::startTimer("Program");

    std::string fileName = findWorkingDir() + "\\index.pi";
    std::cout << fileName << std::endl;
    std::string fileContent = readFile(fileName);

    System::init();

    Scope* scope = new Scope(fileContent);
    scope->setAsMainScope();
    scope->run();
    delete scope;

    std::cout << std::endl;
    Logger::endTimer("Program");

    return 0;
}



/**
 * @author Ozan Akın
 * Pilang Interpretter
 */

#include <iostream>
#include <ctime>
#include <string>
#include <cstdio>
#include <cerrno>

#include "system.h"
#include "scope.h"
#include "stringutils.h"

std::string readFile(std::string fileName) {
    std::FILE *file = std::fopen(fileName.c_str(), "rb");

    if (file) {
        std::string contents;
        fseek(file, 0, SEEK_END);
        contents.resize(std::ftell(file));
        rewind(file);
        fread(&contents[0], 1, contents.size(), file);
        fclose(file);

        return(contents);
    }

    return nullptr;
}

std::string findWorkingDir() {
    std::string dir = __FILE__;
    size_t index = dir.find_last_of('/');

    if (index != std::string::npos) {
        return StringUtils::replaceMiddle(dir, "", index, dir.length());
    }

    return dir;
}

int main() {
    clock_t begin = clock();

    std::string fileName = findWorkingDir() + "/index.pi";
    std::string fileContent = readFile(fileName);

    System::init();

    Scope* scope = new Scope(fileContent);
    scope->setAsMainScope();
    scope->run();

    clock_t end = clock();

    std::cout << std::endl << "Program finished in: " << (double(end - begin) / 1000) << std::endl;

    delete scope;

    return 0;
}



#include <fstream>
#include <iostream>

#include "Error.hpp"
#include "FileLoader.hpp"
#include "Instruction.hpp"


bool FileLoader::load_file(const char* filename)
{
    std::ifstream file(filename);
    if (file) {
        Instruction parser;
        std::string line;
        while (std::getline(file, line)) {
            if (parser.tokenize(line)) {
                parser.eval();
            }
        }
        file.close();
        return true;
    }
    return false;
}

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
        size_t line_number = 1;
        while (std::getline(file, line)) {
            try {
                if (parser.tokenize(line)) {
                    parser.eval();
                }
            }
            catch (Error& error) {
                std::cerr << "File \"" << filename << "\", line " << line_number << std::endl;
                std::cerr << "\t" << line << std::endl;
                std::cerr << error.what() << std::endl;
                file.close();
                return false;
            }
            ++line_number;
        }
        file.close();
        return true;
    }
    std::cerr << "Can't load file '" << filename << "'" << std::endl;
    return false;
}

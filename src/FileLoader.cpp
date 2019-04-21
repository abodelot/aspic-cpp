#include <fstream>
#include <iostream>

#include "Error.hpp"
#include "FileLoader.hpp"
#include "Parser.hpp"


bool FileLoader::load_file(const char* filename)
{
    std::ifstream file(filename);
    if (file) {
        Parser parser;
        std::string line;
        try {
            size_t line_number = 1;
            while (std::getline(file, line)) {
                parser.feed(line);
                ++line_number;
            }
            parser.eval_ast();
        }
        catch (Error& error) {
            // Dump exception to stderr and exit
            // FIXME: track filename / linenumber in AST
            // std::cerr << "File \"" << filename << "\", line " << line_number << std::endl;
            // std::cerr << "\t" << line << std::endl;
            std::cerr << error.what() << std::endl;
            file.close();
            return false;
        }
        file.close();
        return true;
    }
    std::cerr << "Can't load file '" << filename << "'" << std::endl;
    return false;
}

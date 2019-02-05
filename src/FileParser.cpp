#include <fstream>
#include <iostream>
#include "Error.hpp"
#include "FileParser.hpp"
#include "Instruction.hpp"
#include "SymbolTable.hpp"

#define COMMENT_SYMBOL '#'


FileParser::FileParser()
{
}

bool FileParser::load_file(const char* filename)
{
    std::ifstream file(filename);

    if (file)
    {
        Instruction parser;
        std::string line;
        while (std::getline(file, line))
        {
            clear_line(line);
            if (line.size() > 0)
            {
                parser.eval(line);
            }
        }
        file.close();
        return true;
    }
    return false;
}

void FileParser::shell_mode()
{
    Instruction parser;
    std::string input;
    puts("Type expressions for the interpreter to evaluate, or one of the following commands:\n"
         " * exit : exit interpreter\n"
         " * pool : print list of entries in symbol table\n"
         " * debug: print last expression in infix and postfix forms\n");

    bool running = true;
    do
    {
        std::cout << ">> ";
        std::getline(std::cin, input);
        clear_line(input);

        if (input == "exit")
        {
            running = false;
        }
        else if (input == "pool")
        {
            SymbolTable::debug();
        }
        else if (input == "debug")
        {
            parser.debug();
        }
        else if (input.size() > 0)
        {
            try
            {
                parser.eval(input).print_value(std::cout);
                std::cout << std::endl;
            }
            catch (Error& e)
            {
                std::cout << e.what() << std::endl;
            }
        }
    } while (running);
}

void FileParser::clear_line(std::string& line)
{
    // Trim whitespaces
    const char* WHITESPACES = " \t\n\r\0xb";
    size_t first = line.find_first_not_of(WHITESPACES);
    if (first != std::string::npos)
    {
        size_t last = line.find_last_not_of(WHITESPACES);
        line = line.substr(first, last - first + 1);
    }
    // Erase from first comment symbol
    first = line.find(COMMENT_SYMBOL);
    if (first != std::string::npos)
    {
        line.erase(first);
    }
}

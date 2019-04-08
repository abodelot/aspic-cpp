#include <iostream>
#include <readline/readline.h>
#include <readline/history.h>

#include "Shell.hpp"
#include "Instruction.hpp"
#include "Error.hpp"
#include "SymbolTable.hpp"


void Shell::run()
{
    Instruction parser;
    std::cout << "Aspic (" __DATE__ ", " __TIME__ ")" << std::endl;
    std::cout << "Type expressions for the interpreter to evaluate, or one of the following commands:" << std::endl;
    std::cout << " * exit : exit interpreter" << std::endl;
    std::cout << " * pool : print list of entries in symbol table"  << std::endl;
    std::cout << " * lexer: print last parsed expression" << std::endl;

    // Configure readline to insert tabs (instead of PATH completion)
    rl_bind_key('\t', rl_insert);

    bool running = true;
    char* buffer = nullptr;
    while (running && (buffer = readline(">> ")) != nullptr) {
        std::string input(buffer);
        if (strlen(buffer) > 0) {
            add_history(buffer);
        }

        if (input == "exit") {
            running = false;
        }
        else if (input == "pool") {
            SymbolTable::print_all_symbols();
        }
        else if (input == "debug") {
            parser.debug();
        }
        else {
            try {
                std::cout << parser.eval(input) << std::endl;
            }
            catch (Error& e) {
                std::cout << e.what() << std::endl;
            }
        }
        free(buffer);
    }
}

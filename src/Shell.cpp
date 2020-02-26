#include <iostream>
#include <readline/readline.h>
#include <readline/history.h>

#include "Shell.hpp"
#include "Parser.hpp"
#include "Error.hpp"
#include "SymbolTable.hpp"


void Shell::run()
{
    Parser parser;
    std::cout << "Aspic (" __DATE__ ", " __TIME__ ")" << std::endl;
    std::cout << "Type expressions for the interpreter to evaluate, or one of the following commands:" << std::endl;
    std::cout << " * exit:    exit interpreter" << std::endl;
    std::cout << " * scanner: print list of scanned tokens" << std::endl;
    std::cout << " * pool:    print list of entries in symbol table"  << std::endl;
    std::cout << " * ast:     print abstract syntax tree of last expression" << std::endl;

    // Configure readline to insert tabs (instead of PATH completion)
    rl_bind_key('\t', rl_insert);

    bool running = true;
    bool full_statement = true;
    char* buffer = nullptr;
    const char* prompt = ">> ";
    while (running && (buffer = readline(prompt)) != nullptr) {
        std::string input(buffer);
        if (strlen(buffer) > 0) {
            add_history(buffer);
        }

        if (input == "exit") {
            running = false;
        }
        else if (input == "scanner") {
            parser.print_tokens();
        }
        else if (input == "pool") {
            SymbolTable::inspect_symbols();
        }
        else if (input == "ast") {
            parser.print_ast();
        }
        else if (input == "mem") {
            SymbolTable::inspect_memory();
        }
        else if (input == "gc") {
            SymbolTable::mark_and_sweep();
        }
        else {
            // Do not reset parser if user if typing a block
            if (full_statement) {
                parser.reset();
            }
            try {
                full_statement = parser.tokenize(input);
                if (full_statement) {
                    parser.build_ast();
                    const Object& result = parser.eval_ast();
                    // Print result, if any
                    if (result.get_type() != Object::NULL_VALUE) {
                        std::cout << result << std::endl;
                    }
                }
            }
            catch (Error& e) {
                std::cout << e.what() << std::endl;
            }
        }
        free(buffer);
        prompt = full_statement ? ">> " : ".. ";
    }
}

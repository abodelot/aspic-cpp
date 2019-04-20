/**
 * Aspic: an expression interpreter
 * See README.md for more details
 * @author Alexandre Bodelot <alexandre.bodelot@gmail.com>
 */

#include "Shell.hpp"
#include "FileLoader.hpp"
#include "SymbolTable.hpp"


int main(int argc, char* argv[])
{
    SymbolTable::register_stdlib();
    if (argc == 1) {
        Shell shell;
        shell.run();
    }
    else {
        FileLoader loader;
        if (!loader.load_file(argv[1])) {
            return 1;
        }
    }
    return 0;
}

/**
 * Aspic: an instruction interpreter using the Shunting-yard algorithm
 * See README.md for more details
 * @author Alexandre Bodelot <alexandre.bodelot@gmail.com>
 */

/*
TODO List:
----------

- fonctions à arguments variables?? (http://www.kallisti.net.nz/blog/2008/02/extension-to-the-shunting-yard-algorithm-to-allow-variable-numbers-of-arguments-to-functions/)
- mot clefs
- bugfix typeof avec variables
*/

#include <iostream>
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
            std::cerr << "Can't load file '" << argv[1] << "'\n";
        }
        return -1;
    }
    return 0;
}


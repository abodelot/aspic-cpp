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
#include "FileParser.hpp"
#include "SymbolTable.hpp"

int main(int argc, char* argv[])
{
	SymbolTable::register_stdlib();
	FileParser parser;
	if (argc == 1)
	{
		parser.shell_mode();
	}
	else if (parser.load_file(argv[1]))
	{

	}
	else
	{
		std::cerr << "Can't load file '" << argv[1] << "'\n";
	}
	return 0;
}


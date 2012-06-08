/**
 * @author Alexandre Bodelot <alexandre.bodelot@gmail.com>
 * @date 2011-04-15
 */

/*
TODO List:
----------

- opérateurs binaries pour les float
- remplacer Token::VARIABLE et Token::FUNCTION par Token::IDENTIFIER ??
- fonctions à arguments variables?? (http://www.kallisti.net.nz/blog/2008/02/extension-to-the-shunting-yard-algorithm-to-allow-variable-numbers-of-arguments-to-functions/)
- mot clefs
*/

#include <iostream>
#include "FileParser.hpp"


int main(int argc, char* argv[])
{
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


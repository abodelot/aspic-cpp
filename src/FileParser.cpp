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
	puts("Saisir une instruction, ou :\n"
	     " 'exit': quitter\n"
	     " 'vars': afficher les variables en mémoire\n"
	     " 'tokens': afficher les tokens de l'expression infixe\n"
	     " 'postfix': afficher la notation postfixe de la derniere ligne\n");

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
		else if (input == "vars")
		{
			SymbolTable::debug();
		}
		else if (input == "tokens")
		{
			parser.debug_tokens();
		}
		else if (input == "postfix")
		{
			parser.debug_postfix();
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
				std::cout << "[Error] " << e.what() << std::endl;
			}
		}
	} while (running);
}


void FileParser::clear_line(std::string& line)
{
	const char* WHITESPACES = " \t\n\r\0xb";
	size_t first = line.find_first_not_of(WHITESPACES);
	if (first != std::string::npos)
	{
		size_t last = line.find_last_not_of(WHITESPACES);
		line = line.substr(first, last - first + 1);
	}
	first = line.find(COMMENT_SYMBOL);
	if (first != std::string::npos)
	{
		line.erase(first);
	}
}

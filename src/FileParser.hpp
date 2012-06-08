#ifndef FILEPARSER_HPP
#define FILEPARSER_HPP


#include "SymbolTable.hpp"
#include "Error.hpp"

class FileParser
{
public:
	FileParser();

	bool load_file(const char* filename);

	void shell_mode();

private:
	void clear_line(std::string& line);

	SymbolTable symbols_;
};

#endif // FILEPARSER_HPP

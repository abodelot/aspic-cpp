#include <cassert>
#include <iostream>
#include <sstream>

#include "SymbolTable.hpp"
#include "Token.hpp"
#include "Variable.hpp"
#include "Error.hpp"

#include "functions/LibCore.hpp"
#include "functions/LibString.hpp"


SymbolTable::SymbolTable()
{
	// load core library
	add("print", std_print);
	add("input", std_input);
	add("typeof", std_typeof);

	// load string library
	add("str_len", str_len);
	add("str_count", str_count);
	add("str_replace", str_replace);
	add("str_substr", str_substr);
	add("str_trim", str_trim);
	add("str_lower", str_lower);
	add("str_upper", str_upper);
}


SymbolTable::~SymbolTable()
{
	for (VarMap::iterator it = variables_.begin(); it != variables_.end(); ++it) {
		delete it->second;
	}
}


// variables

bool SymbolTable::is_variable(const std::string& name) const
{
	VarMap::const_iterator it = variables_.find(name);
	return it != variables_.end();
}


bool SymbolTable::is_function(const std::string& name) const
{
	FunctionMap::const_iterator it = functions_.find(name);
	return it != functions_.end();
}

Variable* SymbolTable::get(const std::string& name)
{
	return variables_[name];
}

SymbolTable::Function SymbolTable::get_function(const std::string& name) const
{
	FunctionMap::const_iterator it = functions_.find(name);
	return it != functions_.end() ? it->second : NULL;
}


void SymbolTable::add(const std::string& name, Variable* variable)
{
	assert(!is_variable(name));
	variables_[name] = variable;
}

// functions

void SymbolTable::add(const std::string& name, Function function)
{
	functions_[name] = function;
}


void SymbolTable::check_args(TokenStack& args, int count)
{
	if ((int) args.size() < count)
	{
		// "function takes exactaly N argument[s] (Z given)"
		std::ostringstream oss;
		oss << "function takes exactly ";
		if (count == 0)
			oss << "no";
		else
			oss << args.size();

		oss << " argument";
		if (count != 1)
			oss << "s";

		oss << " (" << args.size() << " given)";
		throw Error::SyntaxError(oss.str());
	}
}


void SymbolTable::debug() const
{
	for (VarMap::const_iterator it = variables_.begin(); it != variables_.end(); ++it)
	{
		std::cout << it->first.c_str() << ": ";
		Variable* var = it->second;
		if (var->is_null())
			std::cout << "nil!";
		else
			var->value().print(std::cout);
		std::cout << "\n";
	}
}


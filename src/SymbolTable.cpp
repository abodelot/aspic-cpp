#include <iostream>
#include <sstream>

#include "SymbolTable.hpp"
#include "Error.hpp"
#include "functions/LibCore.hpp"
#include "functions/LibString.hpp"

SymbolTable::VariableMap SymbolTable::variables_;
SymbolTable::FunctionMap SymbolTable::functions_;


void SymbolTable::register_stdlib()
{
	// Load core library (no prefix)
	add("print", std_print);
	add("input", std_input);
	add("typeof", std_typeof);
	add("round", std_round);

	// Load string library
	add("str_len", str_len);
	add("str_count", str_count);
	add("str_replace", str_replace);
	add("str_substr", str_substr);
	add("str_trim", str_trim);
	add("str_lower", str_lower);
	add("str_upper", str_upper);
}


const Function* SymbolTable::get_function(const std::string& name)
{
	FunctionMap::const_iterator it = functions_.find(name);
	return it != functions_.end() ? &it->second : NULL;
}


Variable* SymbolTable::get_variable(const std::string& name)
{
	return &variables_[name];
}


void SymbolTable::add(const std::string& name, Function::Ptr function)
{
	functions_[name] = Function(function);
}


void SymbolTable::check_args(TokenStack& args, int count)
{
	if ((int) args.size() < count)
	{
		// "function takes [exactaly N|no] argument[s] (Z given)"
		std::ostringstream oss;
		oss << "function takes ";
		if (count == 0)
			oss << "no";
		else
			oss << "exactly " << count;

		oss << " argument";
		if (count != 1)
			oss << "s";

		oss << " (" << args.size() << " given)";
		throw Error::SyntaxError(oss.str());
	}
}


void SymbolTable::debug()
{
	for (VariableMap::iterator it = variables_.begin(); it != variables_.end(); ++it)
	{
		std::cout << it->first.c_str() << ": ";
		try
		{
			it->second.get().print_value(std::cout);
		}
		catch (Error& error) // TODO: NameError
		{
			std::cout << "[not initialized yet]";
		}
		std::cout << "\n";
	}
	std::cout << "****** total: " << variables_.size() << " ******" << std::endl;
}

// Debug -----------------------------------------------------------------------

std::string SymbolTable::find_variable_name(const Variable* var)
{
	for (VariableMap::const_iterator it = variables_.begin(); it != variables_.end(); ++it)
	{
		if (&it->second == var)
			return it->first;
	}
	return "<unknown variable>";
}


std::string SymbolTable::find_function_name(const Function* func)
{
	for (FunctionMap::const_iterator it = functions_.begin(); it != functions_.end(); ++it)
	{
		if (&it->second == func)
			return it->first;
	}
	return "<unknown function>";
}

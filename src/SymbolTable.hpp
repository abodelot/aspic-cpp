#ifndef SYMBOLTABLE_HPP
#define SYMBOLTABLE_HPP

#include <string>
#include <map>

#include "Token.hpp"
#include "Variable.hpp"

/**
 * Interpreter symbol table
 * Store each declared identifier (variables or functions)
 */
class SymbolTable
{
public:
	/**
	 * Initialize the symbol table with the function from the standard library
	 */
	static void register_stdlib();

	/**
	 * Retrieve a function definition
	 * @param name: function identifier
	 */
	static const Function* get_function(const std::string& name);

	/**
	 * Get a variable or declare it doesn't already exist
	 * @param name: variable identifier
	 */
	static Variable* get_variable(const std::string& name);

	/**
	 * Declare a C++ function
	 * @param name: function identifier
	 * @param function: pointer to C++ function to store
	 */
	static void add(const std::string& name, Function::Ptr function);

	/**
	 * Print symbol table content
	 */
	static void debug();

	// Debug -------------------------------------------------------------------

	static std::string find_variable_name(const Variable* var);
	static std::string find_function_name(const Function* func);

private:
	SymbolTable();

	typedef std::map<std::string, Variable> VariableMap;
	static VariableMap variables_;

	typedef std::map<std::string, Function> FunctionMap;
	static FunctionMap functions_;
};

#endif // SYMBOLTABLE_HPP

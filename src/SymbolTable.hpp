#ifndef SYMBOLTABLE_HPP
#define SYMBOLTABLE_HPP

#include <string>
#include <map>
#include <stack>


class Variable;
class Token;
typedef std::stack<Token> TokenStack;

/**
 * Interpreter symbol table
 * Store each declared identifier (variables or functions)
 */
class SymbolTable
{
public:
	/**
	 * Define functions signature
	 * the token stack holds all function arguments
	 */
	typedef Token (*Function)(TokenStack&);

	SymbolTable();
	~SymbolTable();

	/**
	 * Test if an indentifier already exists
	 */
	bool is_variable(const std::string& name) const;
	bool is_function(const std::string& name) const;

	/**
	 * Retrieve a variable content
	 * @param name: variable identifier
	 */
	Variable* get(const std::string& name);

	/**
	 * Retrieve a function definition
	 * @param name: function identifier
	 */
	Function get_function(const std::string& name) const;

	/**
	 * Declare a new variable
	 * @param name: variable identifier
	 * @param variable: pointer to variable to store
	 */
	void add(const std::string& name, Variable* variable);

	/**
	 * Declare a C++ function
	 * @param name: function identifier
	 * @param function: pointer to C++ function to store
	 */
	void add(const std::string& name, Function function);

	/**
	 * Print symbol table content
	 */
	void debug() const;

	/**
	 * Raises an exception if arguments count is insufficient
	 * @param args: postfixed tokens
	 * @param count: expected arguments count
	 */
	static void check_args(TokenStack& args, int count);

private:
	typedef std::map<std::string, Variable*> VarMap;
	VarMap variables_;

	typedef std::map<std::string, Function> FunctionMap;
	FunctionMap functions_;


};

#endif // SYMBOLTABLE_HPP
